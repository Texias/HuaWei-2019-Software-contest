#include"car_road_cross.h"
using namespace std;
void exchange_char(char &a, char &b)
{
    char c=a;
    a=b;
    b=c;
}
void exchange(int &a,int &b)/**********交换两个数的值************/
{
    int temp=a;
    a=b;
    b=temp;
}
/*******************设置每个路口的入口道路升序和邻通路口******************/
void set_cross(map<int,Cross>& cross_map,map<int,Road>& road_map)
{
    map<int,Cross>::iterator pt_cross_map=cross_map.begin();
    while(pt_cross_map!=cross_map.end())
    {
        int cross_id=pt_cross_map->first; //路口ID
        for(int i=0;i!=rand_size;++i)  /******************确定邻通路口*****************/
        {
            int road_id=cross_map[cross_id].road[i];//道路ID
            cross_map[cross_id].in_rand[i]=road_id;
            cross_map[cross_id].other_cross[i]=0;
            if(road_id>0)
            {
                if(road_map[road_id].isduplex==1)
                {
                    if(road_map[road_id].end_cross==cross_id)
                        cross_map[cross_id].other_cross[i]=road_map[road_id].start_cross;
                    else
                        cross_map[cross_id].other_cross[i]=road_map[road_id].end_cross;
                }
                else if(road_map[road_id].start_cross==cross_id)
                {
                    cross_map[cross_id].other_cross[i]=road_map[road_id].end_cross;
                    cross_map[cross_id].in_rand[i]=-1;
                }
            }
        }
        for(int i=rand_size-1;i!=0;--i)  /********排序路口的各个道路***********/
        {
            int Max=0;
            for(int j=1;j<=i;++j)
            {
                if(cross_map[cross_id].in_rand[Max]<cross_map[cross_id].in_rand[j])
                    Max=j;
            }
            if(Max!=i)
                exchange(cross_map[cross_id].in_rand[i],cross_map[cross_id].in_rand[Max]);
        }

        ++pt_cross_map;
    }
}
/**********************************************输入car的路径信息并构建map******************************/
int make_car_multimap(const char *direction,map<int, multimap<int ,Car > >&car_multimap)
{
    ifstream infile(direction);
    if(!infile)
    {
        cerr<<"error: unable to open car file:"<<direction<<endl;
        return -1;
    }
    string line; Car car;char sh;
    int car_number=0;/**车子总的数量**/
    getline(infile,line);
    multimap<int ,Car > car_multi;
    while(getline(infile,line))
    {
        istringstream stream(line);
        stream>>sh>>car.car_ID>>sh>>car.start_cross>>sh>>car.end_cross>>sh>>car.max_speed>>sh>>car.start_time>>sh;
        car_multi.insert(make_pair(car.start_time ,car));
        ++car_number;
    }
    int i=0,j=1;
    for(multimap<int ,Car >::iterator iter=car_multi.begin();iter!=car_multi.end();)
    {

        if(i==multimap_num)
        {
            i=0;
            ++j;
        }
        while(i!=multimap_num&&iter!=car_multi.end())
        {
            car_multimap[j].insert(make_pair(iter->first, iter->second));
            ++iter;
            ++i;
        }
    }
    infile.close();
    return car_number;
}
/*********************输出各辆车的路径****************************/
void output(const char* direction, map<int,list<int> >&car_map)
{
    ofstream outfile(direction);
    if(!outfile)
    {
        cerr<<"error: unable to open outfile:"<<direction<<endl;

    }

    outfile<<"#(carId,StartTime,RoadId...)\n";
    for(map<int,list<int> >::iterator iter =car_map.begin();iter!=car_map.end();++iter)
    {
        outfile<<'('<<iter->second.front() ;/****/
        iter->second.pop_front();
        while(!iter->second.empty())
        {
            outfile<<", "<<iter->second.front();
            iter->second.pop_front();
        }
        outfile<<")\n";
    }
    outfile.close();
}
/************************************输入cross的信息并构建map*************************************************/
void make_cross_map(const char *direction,map<int ,Cross >&cross_map)
{
    ifstream infile(direction);
    if(!infile)
    {
        cerr<<"error: unable to open cross file:"<<direction<<endl;
        return ;
    }
    string line; Cross cross;char sh;
    getline(infile,line);
    while(getline(infile,line))
    {
        istringstream stream(line);
        stream>>sh>>cross.cross_ID>>sh>>cross.road[0]>>sh>>cross.road[1]>>sh>>cross.road[2]>>sh>>cross.road[3]>>sh;
        cross_map[cross.cross_ID] =cross;
    }
    infile.close();
}
/************************************输入road的信息并构建map*************************************************/
int make_road_map(const char *direction,map<int ,Road >&road_map)
{
    ifstream infile(direction);
    if(!infile)
    {
        cerr<<"error: unable to open road file:"<<direction<<endl;
        return 0;
    }
    string line; Road road;char sh;
    getline(infile,line);
    int number=0;
    while(getline(infile,line))
    {
        istringstream stream(line);
        stream>>sh>>road.road_ID>>sh>>road.road_length>>sh>>road.max_speed>>sh>>road.channel_num>>sh>>road.start_cross>>sh>>road.end_cross>>sh>>road.isduplex>>sh;

        number+=(road.isduplex+1)*road.channel_num*road.road_length;

        road_map[road.road_ID]=road;

        road_map[road.road_ID].road_gesture[road.start_cross]=(int **)new int* [road.channel_num];/***创建以start——cross为起始路口的坑位**/
        for(int i=0;i<road.channel_num;++i)
            road_map[road.road_ID].road_gesture[road.start_cross][i]=new int[road.road_length];
        for(int i=0;i<road.channel_num;++i)
        {
            for(int j=0;j<road.road_length;++j)
                 road_map[road.road_ID].road_gesture[road.start_cross][i][j]=0;  /***初始化道路信息，道路上的任意位置无车辆是置为0***/

        }
        if(road.isduplex==1)/***创建以end——cross为起始路口的坑位**/
        {
            road_map[road.road_ID].road_gesture[road.end_cross]=(int **)new int* [road.channel_num];
            for(int i=0;i<road.channel_num;++i)
                road_map[road.road_ID].road_gesture[road.end_cross][i]=new int[road.road_length];
            for(int i=0;i<road.channel_num;++i)
            {
                for(int j=0;j<road.road_length;++j)
                    road_map[road.road_ID].road_gesture[road.end_cross][i][j]=0;  /***初始化道路信息，道路上的任意位置无车辆是置为0***/
            }

        }
    }
    infile.close();
    return number;
}
/***************************由起始路口和终止路口，返回起始到终止之间的最短街道数目**********************************/
int test_distance(int cross_begin,int cross_end,map<int ,Cross>&cross_map)/***********!!!!!***********/
{
    int Logo=1,distance=0;
    if(cross_begin!=cross_end)/**该路口不为终止路口**/
    {
        list<int> list_one,list_two;
        set<int> sec;/**存放遍历过了的cross**/
        int temp,temps;
        list_one.push_back(cross_begin);
        while(Logo)
        {
            while(!list_one.empty())
            {
                temp=list_one.front();
                list_one.pop_front();
                list_two.push_back(temp);
            }
            while(!list_two.empty())
            {
                temp=list_two.front();
                if(temp==cross_end)
                {
                    Logo=0;
                    break;
                }
                sec.insert(temp);
                list_two.pop_front();
                for(int i=0;i<rand_size;++i)
                {
                    temps=cross_map[temp].other_cross[i];
                    if(temps>0)
                    {
                        if(!sec.count(temps))
                            list_one.push_back(temps);
                    }
                }
            }
            ++distance;
        }
    }
    return distance;
}
/*******************************由cross_now和终点cross确定next_cross[]和next_road[]*******************************************/
void renew_road_cross(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map,map<int,Road>& road_map)
{
    int begins=car_map[car_id].cross_now;
    int ends=car_map[car_id].end_cross;
    int cross_band[rand_size]; /**下一个路口的优先级排列**/
    for(int i=0;i!=rand_size;++i)  /*********cross_band包含邻接路口到终点的距离，无邻接路口时为0*********/
    {
        if(cross_map[begins].other_cross[i]>0)
            cross_band[i]=test_distance(cross_map[begins].other_cross[i],ends,cross_map);
        else
            cross_band[i]=-1;
    }
    for(int j=0;j!=rand_size;++j)/*********由cross_band得到具有优先级的下一路口数组cross_bands************/
    {
        int Min=-1;
        int least=10000000;
        for(int i=0;i!=rand_size;++i)
        {
            if(cross_band[i]!=-1)
            {
                if(least>cross_band[i])
                    {
                        least=cross_band[i];
                        Min=i;
                    }
            }
        }
        if(Min==-1)
            car_map[car_id].next_cross[j]=0;    /**************0表示没有路口*****************/
        else
        {
            car_map[car_id].next_cross[j]=cross_map[begins].other_cross[Min];
            cross_band[Min]=-1;
        }
    }
   /**确定next_cross***/
    for(int i=0;i!=rand_size;++i)
    {
        int temp=car_map[car_id].next_cross[i];
        if(temp==0)/***没有路口则没有道路*****/
            car_map[car_id].next_road[i]=0;
        else
        {
            for(int j=0;j!=rand_size;++j)
            {
                int temps=cross_map[temp].road[j];
                if(temps>0)
                {
                    if(road_map[temps].start_cross==temp&&road_map[temps].end_cross==begins)
                        car_map[car_id].next_road[i]=temps;
                    if(road_map[temps].end_cross==temp&&road_map[temps].start_cross==begins)
                        car_map[car_id].next_road[i]=temps;
                }
            }
        }
    }
}
/*****转弯情况根据车辆当前所在路口和下一道路得出****/
void renew_direction(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map)/******更新该车在该路口的转弯情况********/
{
    int location=0;
    int temp=car_map[car_id].cross_now;
    for(int i=0;i!=rand_size;++i)
    {
        if(car_map[car_id].road_now==cross_map[temp].road[i])
            location=i;
    }


        if(location==0)
        {
            for(int j=0;j!=rand_size;++j)
            {
                if(car_map[car_id].next_road[j]==0)
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[0])
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[1])
                {
                    car_map[car_id].direction[j]='L';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[2])
                {
                    car_map[car_id].direction[j]='D';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[3])
                {
                    car_map[car_id].direction[j]='R';
                    continue;
                }
            }
        }
        if(location==1)
        {
            for(int j=0;j!=rand_size;++j)
            {
                if(car_map[car_id].next_road[j]==0)
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[0])
                {
                    car_map[car_id].direction[j]='R';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[1])
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[2])
                {
                    car_map[car_id].direction[j]='L';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[3])
                {
                    car_map[car_id].direction[j]='D';
                    continue;
                }
            }
        }
        if(location==2)
        {
            for(int j=0;j!=rand_size;++j)
            {
                if(car_map[car_id].next_road[j]==0)
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[0])
                {
                    car_map[car_id].direction[j]='D';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[1])
                {
                    car_map[car_id].direction[j]='R';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[2])
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[3])
                {
                    car_map[car_id].direction[j]='L';
                    continue;
                }
            }

        }
        if(location==3)
        {
            for(int j=0;j!=rand_size;++j)
            {
                if(car_map[car_id].next_road[j]==0)
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[0])
                {
                    car_map[car_id].direction[j]='L';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[1])
                {
                    car_map[car_id].direction[j]='D';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[2])
                {
                    car_map[car_id].direction[j]='R';
                    continue;
                }
                if(car_map[car_id].next_road[j]==cross_map[temp].road[3])
                {
                    car_map[car_id].direction[j]='N';
                    continue;
                }
            }
        }
}
/*******更新车速********/
void renew_speed_now(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map)
{
    int temp=car_map[car_id].road_now;
    car_map[car_id].speed_now=car_map[car_id].max_speed;
    if(car_map[car_id].max_speed>road_map[temp].max_speed)
        car_map[car_id].speed_now=road_map[temp].max_speed;
}
pair<int, int> next_location(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map)/***first: -2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它：能确定下一位置**/
{
    int road_next=car_map[car_id].next_road[0];
    int the_cross_now=car_map[car_id].cross_now;
    int k=road_map[road_next].max_speed-car_map[car_id].road_locate.second;/**k为下一道路可行驶的距离*/
    if(k<=0)
    {
        return make_pair(-2,0);
    }
    int length=road_map[road_next].road_length-1;
    for(int i=0;i!=road_map[road_next].channel_num;++i)/***/
    {
        int j=0;
        for(j=length;j>length-k;--j)
        {
            if(road_map[road_next].road_gesture[the_cross_now][i][j]!=0)
            {
                int the_car=road_map[road_next].road_gesture[the_cross_now][i][j];
                if(car_map[the_car].is_start==0)/***若为等待，则为等待**/
                {
                    return make_pair(-1,0);
                }

                if(car_map[the_car].is_start==1)/**若为终止***/
                {
                    if(j==length)
                    {
                        if(i==road_map[road_next].channel_num-1)
                        {
                            return make_pair(-2,0);
                        }
                        else
                        {
                            break; /**进入下一车道**/
                        }

                    }
                    else
                    {
                        return make_pair(i,j+1);
                    }
                }
            }
        }
        if(j==length-k)/**无车阻碍**/
        {
            return make_pair(i,j+1);
        }
    }
}

void copy_car_map(map<int ,Car>&real_one , map<int ,Car>&copy_one)  /**复制地图**/
{
    for(map<int ,Car>::iterator iter =real_one.begin();iter!=real_one.end();++iter)
    {
        copy_one[iter->first].car_ID=iter->second.car_ID;
        copy_one[iter->first].start_cross=iter->second.start_cross;
        copy_one[iter->first].end_cross=iter->second.end_cross;
        copy_one[iter->first].max_speed=iter->second.max_speed;
        copy_one[iter->first].start_time=iter->second.start_time;
        copy_one[iter->first].is_start=iter->second.is_start;
        copy_one[iter->first].speed_now=iter->second.speed_now;
        copy_one[iter->first].road_now=iter->second.road_now;
        copy_one[iter->first].cross_now=iter->second.cross_now;
        copy_one[iter->first].road_locate.first=iter->second.road_locate.first;
        copy_one[iter->first].road_locate.second=iter->second.road_locate.second;

        for(int i=0;i!=rand_size;++i)
        {
            copy_one[iter->first].next_road[i]=iter->second.next_road[i];
            copy_one[iter->first].next_cross[i]=iter->second.next_cross[i];
            copy_one[iter->first].direction[i]=iter->second.direction[i];
        }
    }
}

void copy_road_map(map<int ,Road>&real_one , map<int ,Road>&copy_one)
{
    for(map<int ,Road>::iterator iter =real_one.begin();iter!=real_one.end();++iter)
    {
        copy_one[iter->first].road_ID=iter->second.road_ID;
        copy_one[iter->first].road_length=iter->second.road_length;
        copy_one[iter->first].max_speed=iter->second.max_speed;
        copy_one[iter->first].channel_num=iter->second.channel_num;
        copy_one[iter->first].start_cross=iter->second.start_cross;
        copy_one[iter->first].end_cross=iter->second.end_cross;
        copy_one[iter->first].isduplex=iter->second.isduplex;

        copy_one[iter->first].road_gesture[iter->second.start_cross]=(int **)new int* [iter->second.channel_num];/**复制start_cross**/
        for(int i=0;i<iter->second.channel_num;++i)
            copy_one[iter->first].road_gesture[iter->second.start_cross][i]=new int[iter->second.road_length];
        for(int i=0;i!=iter->second.channel_num;++i)
        {
            for(int j=0;j!=iter->second.road_length;++j)
            {
                copy_one[iter->first].road_gesture[iter->second.start_cross][i][j]=iter->second.road_gesture[iter->second.start_cross][i][j];
            }
        }
        if(iter->second.isduplex==1)
        {
        copy_one[iter->first].road_gesture[iter->second.end_cross]=(int **)new int* [iter->second.channel_num];/**复制start_cross**/
        for(int i=0;i<iter->second.channel_num;++i)
            copy_one[iter->first].road_gesture[iter->second.end_cross][i]=new int[iter->second.road_length];
        for(int i=0;i!=iter->second.channel_num;++i)
        {
            for(int j=0;j!=iter->second.road_length;++j)
            {
                copy_one[iter->first].road_gesture[iter->second.end_cross][i][j]=iter->second.road_gesture[iter->second.end_cross][i][j];
            }
        }

        }
    }
}

int test_lock_num(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map)//跑一个调度，返回等待车辆的数目
{
        /**处理在等待的车辆**/
        /**终止一个减一个**/
        while(1)
        {
            int no_change=0;
            int total_wait_num=0;
            for(map<int ,Cross>::iterator iter_cross_map=cross_map.begin();iter_cross_map!=cross_map.end();++iter_cross_map)/**升序调度各个路口**/
            {
                //int cross_id=iter_cross_map->first;
                //int goto_next_cross=0;/**检测是否变更到下一路口**/
                while(1)/**多次循环调度该路口道路直至该路口所有车辆都为终止状态或者循环一遍没有车辆变为终止状态**/
                {
                    int PM2 =0;
                    int wait_num=0;/**遍历一个路口车子处于等待状态的数量**/
                    for(int i=0;i!=rand_size;++i)/**调度该路口各个出路口道路**/
                    {
                        int road_id=iter_cross_map->second.in_rand[i];
                        if(road_id!=-1)
                        {
                            int goto_next_road=0;
                            int the_cross_other=iter_cross_map->first;
                            int the_cross_now;
                            if(the_cross_other==road_map[road_id].start_cross)
                            {
                                the_cross_now=road_map[road_id].end_cross;
                            }
                            else
                            {
                                the_cross_now=road_map[road_id].start_cross;
                            }
                            for(int m=0;m<road_map[road_id].road_length;++m)/**顺序调度道路各个车辆**/
                            {
                                if(goto_next_road)
                                {
                                    break;
                                }
                                for(int n=0;n<road_map[road_id].channel_num;++n)
                                {
                                    if(road_map[road_id].road_gesture[the_cross_now][n][m]!=0)/**找到第一辆车**/
                                    {
                                        int car_id=road_map[road_id].road_gesture[the_cross_now][n][m];
                                        if(car_map[car_id].is_start==0) /***且该车处于等待状态(可以确定该车前面一定无阻挡??????????)**/
                                        {
                                            ++wait_num;
                                            int stock;////判断该车前方是否会有阻挡
                                            for(stock =m;stock!=-1;--stock)
                                            {
                                                int the_car_id =road_map[road_id].road_gesture[the_cross_now][n][stock];
                                                if(the_car_id!=0)
                                                {
                                                    if((m-stock)>road_map[road_id].max_speed)
                                                    {
                                                        stock=-1;
                                                        break;
                                                    }
                                                    if(car_map[the_car_id].is_start==1)
                                                    {
                                                        break;
                                                    }
                                                }
                                            }
                                            if(stock==-1)/**且该车前面无阻挡！！！！！！！！！*/
                                            {



                                            if(car_map[car_id].speed_now>m)/**能出道路**/
                                            {
                                                if(car_map[car_id].cross_now==car_map[car_id].end_cross)/**路口为终止路口**/
                                                {
                                                    //更新信息，调度下一辆车
                                                    car_map[car_id].is_start= 2;   /**到达终点！！！！！！！！！！！！！！！**/
                                                    road_map[road_id].road_gesture[the_cross_now][n][m]= 0;/**更新道路状态**/
                                                    --no_change;
                                                    --PM2;
                                                    --wait_num;
                                                    continue;   /**调度下一辆车 **/
                                                }
                                                //不为终止路口，则判断左右转直行优先级
                                                if(car_map[car_id].direction[0]=='D')/**直行*/
                                                {
                                                    pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它:下一位置**/
                                                    if(P.first>=0)/**能进入下一道路**/
                                                    {
                                                        //更新信息！！！！！！，调度下一辆车
                                                        car_map[car_id].road_locate=P;/**更新车辆状态中的pair**/
                                                        car_map[car_id].is_start=1;/**更新为终止状态**/
                                                        car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                        car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                        renew_speed_now(car_id,car_map,road_map);
                                                        renew_road_cross(car_id,car_map,cross_map,road_map);
                                                        renew_direction(car_id,car_map,cross_map);
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                        road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue; /**下一辆车**/
                                                    }
                                                    if(P.first==-1)/**不能进入下一道路**/
                                                    {
                                                        //仍为等待，调度下一道路!
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    if(P.first==-2)/**确定终止，停在当前道路 **/
                                                    {
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                        road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                        car_map[car_id].is_start=1;
                                                        car_map[car_id].road_locate.second=0;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue;/**下一辆车**/
                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='L')/**左转*/
                                                {
                                                    //判断该不该它走，若不该，标为等待，开始下一道路
                                                    //若该他走，判断能否进入下一道路
                                                    //能进入下一道路，更新信息！！！！！！调度下一辆车
                                                    //不能进入下一道路,标为等待
                                                    int i=0;/**由i的值来确定是否需要等待 1：等待 0：不用等待**/
                                                    for(int j=0;j!=rand_size;++j)
                                                    {
                                                        if(i==1)
                                                        {
                                                            break;
                                                        }
                                                        int Q=0;
                                                        int m=cross_map[iter_cross_map->first].in_rand[j];
                                                        if(m!=-1)
                                                        {
                                                            if(m!=car_map[car_id].road_now)
                                                            {
                                                                for(int pre_i=0;pre_i!=road_map[m].road_length;++pre_i)
                                                                {
                                                                    if(i==1||Q==1)
                                                                    {
                                                                        break;
                                                                    }
                                                                    for(int pre_j=0;pre_j!=road_map[m].channel_num;++pre_j)
                                                                    {
                                                                        if(i==1||Q==1)
                                                                        {
                                                                            break;
                                                                        }
                                                                        int cross_m;
                                                                        if(road_map[m].start_cross==iter_cross_map->first)
                                                                        {
                                                                            cross_m=road_map[m].end_cross;
                                                                        }
                                                                        else
                                                                        {
                                                                            cross_m=road_map[m].start_cross;
                                                                        }
                                                                        int pre_m=road_map[m].road_gesture[cross_m][pre_j][pre_i];
                                                                        if(pre_m!=0)
                                                                        {


                                                                        if(car_map[pre_m].is_start==0)/**该车处于等待**/
                                                                        {
                                                                            if(car_map[pre_m].direction[0]=='D')
                                                                            {
                                                                                if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                                    i= 1; /**表示需要等待**/
                                                                            }
                                                                            Q=1;
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***有其他直行的车辆经过***/
                                                    {
                                                        goto_next_road=1;//仍为等待，开始下一条道路！！！！！！！！！！！！！！！！！！
                                                        break;
                                                    }
                                                    if(i==0)
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它:下一位置**/
                                                        if(P.first>=0)/**能进入下一道路**/
                                                        {
                                                            //更新信息！！！！！！，调度下一辆车
                                                            car_map[car_id].road_locate=P;/**更新车辆状态中的pair**/
                                                            car_map[car_id].is_start=1;/**更新为终止状态**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**下一辆车**/
                                                        }
                                                        if(P.first==-1)/**不能进入下一道路**/
                                                        {
                                                            //仍为等待，调度下一道路!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**确定终止，停在当前道路 **/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**下一辆车**/
                                                        }

                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='R')/**右转**/
                                                {
                                                    //判断该不该它走，若不该，标为等待，开始下一道路
                                                    //若该他走，判断能否进入下一道路
                                                    //能进入下一道路，更新信息！！！！！！调度下一辆车
                                                    //不能进入下一道路,标为等待
                                                    int i=0;/**由i的值来确定是否需要等待 1：等待 0：不用等待**/
                                                    for(int j=0;j!=rand_size;++j)
                                                    {
                                                        if(i==1)
                                                        {
                                                            break;
                                                        }
                                                        int Q=0;
                                                        int m=cross_map[iter_cross_map->first].in_rand[j];
                                                        if(m!=-1)
                                                        {
                                                            if(m!=car_map[car_id].road_now)
                                                            {
                                                                for(int pre_i=0;pre_i!=road_map[m].road_length;++pre_i)
                                                                {
                                                                    if(i==1||Q==1)
                                                                    {
                                                                        break;
                                                                    }
                                                                    for(int pre_j=0;pre_j!=road_map[m].channel_num;++pre_j)
                                                                    {
                                                                        if(i==1||Q==1)
                                                                        {
                                                                            break;
                                                                        }
                                                                        int cross_m;
                                                                        if(road_map[m].start_cross==iter_cross_map->first)
                                                                        {
                                                                            cross_m=road_map[m].end_cross;
                                                                        }
                                                                        else
                                                                        {
                                                                            cross_m=road_map[m].start_cross;
                                                                        }
                                                                        int pre_m=road_map[m].road_gesture[cross_m][pre_j][pre_i];
                                                                        if(pre_m!=0)
                                                                        {


                                                                        if(car_map[pre_m].is_start==0)/**该车处于等待**/
                                                                        {
                                                                            if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                            {
                                                                                if(car_map[pre_m].direction[0]=='D'||car_map[pre_m].direction[0]=='L')

                                                                                    i= 1; /****表示需要等待**/
                                                                            }
                                                                            Q=1;
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***有其他直行或者左转的车辆经过***/
                                                    {
                                                        //仍为等待，开始下一条道路！！！！！！！！！！！！！！！！！！
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它:下一位置**/
                                                        if(P.first>=0)/**能进入下一道路**/
                                                        {
                                                            //更新信息！！！！！！，调度下一辆车
                                                            car_map[car_id].road_locate=P;/**更新车辆状态中的pair**/
                                                            car_map[car_id].is_start=1;/**更新为终止状态**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**下一辆车**/
                                                        }
                                                        if(P.first==-1)/**不能进入下一道路**/
                                                        {
                                                            //仍为等待，调度下一道路!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**确定终止，停在当前道路 ***/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**下一辆车**/
                                                        }

                                                    }

                                                }
                                            }
                                            else/**不能出道路**/
                                            {
                                                //更新车辆、道路信息
                                                car_map[car_id].is_start=1;
                                                car_map[car_id].road_locate.second=m-car_map[car_id].speed_now;
                                                road_map[road_id].road_gesture[the_cross_now][n][m]=0;
                                                road_map[road_id].road_gesture[the_cross_now][n][m-car_map[car_id].speed_now]=car_id;
                                                --PM2;
                                                --no_change;
                                                --wait_num;
                                                continue;//遍历该道路下一辆车
                                            }
                                            }
                                            else////////该车前面有阻挡(stock+1)为它的归属
                                            {
                                                car_map[car_id].is_start=1;
                                                car_map[car_id].road_locate.second=stock+1;
                                                road_map[road_id].road_gesture[the_cross_now][n][m]=0;
                                                road_map[road_id].road_gesture[the_cross_now][n][stock+1]=car_id;
                                                --PM2;
                                                --no_change;
                                                --wait_num;
                                                continue;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(wait_num==0)
                    {
                        break;
                    }
                    if(PM2==0)/***有等待状态且无车终止**/
                    {
                        total_wait_num+=wait_num;
                        break; //调度下一个路口
                    }
                }
            }

            if(total_wait_num==0)
            {
                return 0;
            }
            if(no_change==0)/**出现死锁，返回该地图目前处于等待状态的车辆的数目*/
            {
                int number =0;
                int car_id =0;
                for(map<int ,Road>::iterator iter=road_map.begin();iter!=road_map.end();++iter)
                {
                    for(int i=0;i!=road_map[iter->first].channel_num;++i)
                    {
                        for(int j=0;j!=road_map[iter->first].road_length;++j)
                        {
                            for(map<int ,int **>::iterator pt_cross=road_map[iter->first].road_gesture.begin();pt_cross!=road_map[iter->first].road_gesture.end();++pt_cross)
                            {
                            car_id=pt_cross->second[i][j];
                            if(car_id!=0)
                            {
                                if(car_map[car_id].is_start==0)
                                {
                                    ++number;
                                }
                            }

                            }

                        }
                    }
                }
                return number;
            }
        }
}


/**测试是否死锁first>=1表示出现死锁，且second为可以解决死锁的车辆id；first为0表示不会出现死锁**/
pair<int ,int > test_lock(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map)
{
        map<int ,Car> car_map_copy ,car_map_copy_one;
        map<int ,Road> road_map_copy, road_map_copy_one;
        copy_car_map(car_map , car_map_copy);
        copy_car_map(car_map , car_map_copy_one);
        copy_road_map(road_map , road_map_copy);
        copy_road_map(road_map , road_map_copy_one);
        /**处理在等待的车辆**/
        /**终止一个减一个**/
        while(1)
        {
            int no_change=0;
            int total_wait_num=0;
            for(map<int ,Cross>::iterator iter_cross_map=cross_map.begin();iter_cross_map!=cross_map.end();++iter_cross_map)/**升序调度各个路口**/
            {
                //int cross_id=iter_cross_map->first;
                //int goto_next_cross=0;/**检测是否变更到下一路口**/

                while(1)/**多次循环调度该路口道路直至该路口所有车辆都为终止状态或者循环一遍没有车辆变为终止状态**/
                {
                    int PM2=0;
                    int wait_num=0;/**遍历一个路口车子处于等待状态的数量**/
                    for(int i=0;i!=rand_size;++i)/**调度该路口各个出路口道路**/
                    {
                        int road_id=iter_cross_map->second.in_rand[i];
                        if(road_id!=-1)
                        {
                            int goto_next_road=0;
                            int the_cross_other=iter_cross_map->first;
                            int the_cross_now;
                            if(iter_cross_map->first==road_map[road_id].start_cross)
                            {
                                the_cross_now=road_map[road_id].end_cross;
                            }
                            else
                            {
                                the_cross_now=road_map[road_id].start_cross;
                            }
                            for(int m=0;m<road_map[road_id].road_length;++m)/**顺序调度道路各个车辆**/
                            {
                                if(goto_next_road==1)
                                {
                                    break;
                                }
                                for(int n=0;n<road_map[road_id].channel_num;++n)
                                {
                                    if(road_map[road_id].road_gesture[the_cross_now][n][m]!=0)/**找到第一辆车**/
                                    {
                                        int car_id=road_map[road_id].road_gesture[the_cross_now][n][m];

                                        if(car_map[car_id].is_start==0) /***且该车处于等待状态(可以确定该车前面一定无阻挡??????????)**/
                                        {
                                            ++wait_num;
                                            int stock;////判断该车前方是否会有阻挡
                                            for(stock =m;stock!=-1;--stock)
                                            {
                                                int the_car_id =road_map[road_id].road_gesture[the_cross_now][n][stock];

                                                if(the_car_id!=0)
                                                {
                                                    if((m-stock)>road_map[road_id].max_speed)
                                                    {
                                                        stock=-1;
                                                        break;
                                                    }
                                                    if(car_map[the_car_id].is_start==1)
                                                    {
                                                        break;
                                                    }
                                                }
                                            }
                                            if(stock==-1)/**且该车前面无阻挡！！！！！！！！！*/
                                            {
                                            if(car_map[car_id].speed_now>m)/**能出道路**/
                                            {
                                                if(car_map[car_id].cross_now==car_map[car_id].end_cross)/**路口为终止路口**/
                                                {
                                                    //更新信息，调度下一辆车
                                                    car_map[car_id].is_start= 2;   /**到达终点！！！！！！！！！！！！！！！**/
                                                    road_map[road_id].road_gesture[the_cross_now][n][m]= 0;/**更新道路状态**/
                                                    --no_change;
                                                    --PM2;
                                                    --wait_num;
                                                    continue;   /**调度下一辆车 **/
                                                }
                                                //不为终止路口，则判断左右转直行优先级
                                                if(car_map[car_id].direction[0]=='D')/**直行*/
                                                {
                                                    pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它:下一位置**/
                                                    if(P.first>=0)/**能进入下一道路**/
                                                    {
                                                        //更新信息！！！！！！，调度下一辆车
                                                        car_map[car_id].road_locate=P;/**更新车辆状态中的pair**/
                                                        car_map[car_id].is_start=1;/**更新为终止状态**/
                                                        car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                        car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                        renew_speed_now(car_id,car_map,road_map);
                                                        renew_road_cross(car_id,car_map,cross_map,road_map);
                                                        renew_direction(car_id,car_map,cross_map);
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                        road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue; /**下一辆车**/
                                                    }
                                                    if(P.first==-1)/**不能进入下一道路**/
                                                    {
                                                        //仍为等待，调度下一道路!
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    if(P.first==-2)/**确定终止，停在当前道路 **/
                                                    {
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                        road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                        car_map[car_id].is_start=1;
                                                        car_map[car_id].road_locate.second=0;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue;/**下一辆车**/
                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='L')/**左转*/
                                                {
                                                    //判断该不该它走，若不该，标为等待，开始下一道路
                                                    //若该他走，判断能否进入下一道路
                                                    //能进入下一道路，更新信息！！！！！！调度下一辆车
                                                    //不能进入下一道路,标为等待
                                                    int i=0;/**由i的值来确定是否需要等待 1：等待 0：不用等待**/
                                                    for(int j=0;j!=rand_size;++j)
                                                    {
                                                        if(i==1)
                                                        {
                                                            break;
                                                        }
                                                        int Q=0;
                                                        int m=cross_map[iter_cross_map->first].in_rand[j];
                                                        if(m!=-1)
                                                        {
                                                            if(m!=car_map[car_id].road_now)
                                                            {
                                                                for(int pre_i=0;pre_i!=road_map[m].road_length;++pre_i)
                                                                {
                                                                    if(i==1||Q==1)
                                                                    {
                                                                        break;
                                                                    }
                                                                    for(int pre_j=0;pre_j!=road_map[m].channel_num;++pre_j)
                                                                    {
                                                                        if(i==1||Q==1)
                                                                        {
                                                                            break;
                                                                        }

                                                                        int cross_m;
                                                                        if(road_map[m].start_cross==iter_cross_map->first)
                                                                        {
                                                                            cross_m=road_map[m].end_cross;
                                                                        }
                                                                        else
                                                                        {
                                                                            cross_m=road_map[m].start_cross;
                                                                        }
                                                                        int pre_m=road_map[m].road_gesture[cross_m][pre_j][pre_i];
                                                                        if(pre_m!=0)
                                                                        {
                                                                        if(car_map[pre_m].is_start==0)/**该车处于等待**/
                                                                        {
                                                                            if(car_map[pre_m].direction[0]=='D')
                                                                            {
                                                                                if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                                    i= 1; /**表示需要等待**/
                                                                            }
                                                                            Q=1;
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***有其他直行的车辆经过***/
                                                    {
                                                        goto_next_road=1;//仍为等待，开始下一条道路！！！！！！！！！！！！！！！！！！
                                                        break;
                                                    }
                                                    if(i==0)
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它:下一位置**/
                                                        if(P.first>=0)/**能进入下一道路**/
                                                        {
                                                            //更新信息！！！！！！，调度下一辆车
                                                            car_map[car_id].road_locate=P;/**更新车辆状态中的pair**/
                                                            car_map[car_id].is_start=1;/**更新为终止状态**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**下一辆车**/
                                                        }
                                                        if(P.first==-1)/**不能进入下一道路**/
                                                        {
                                                            //仍为等待，调度下一道路!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**确定终止，停在当前道路 **/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**下一辆车**/
                                                        }

                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='R')/**右转**/
                                                {
                                                    //判断该不该它走，若不该，标为等待，开始下一道路
                                                    //若该他走，判断能否进入下一道路
                                                    //能进入下一道路，更新信息！！！！！！调度下一辆车
                                                    //不能进入下一道路,标为等待
                                                    int i=0;/**由i的值来确定是否需要等待 1：等待 0：不用等待**/
                                                    for(int j=0;j!=rand_size;++j)
                                                    {

                                                        if(i==1)
                                                        {
                                                            break;
                                                        }
                                                        int Q=0;

                                                        int m=iter_cross_map->second.in_rand[j];

                                                        if(m!=-1)
                                                        {
                                                            if(m!=car_map[car_id].road_now)
                                                            {
                                                                for(int pre_i=0;pre_i!=road_map[m].road_length;++pre_i)
                                                                {
                                                                    if(i==1||Q==1)
                                                                    {
                                                                        break;
                                                                    }
                                                                    for(int pre_j=0;pre_j!=road_map[m].channel_num;++pre_j)
                                                                    {
                                                                        if(i==1||Q==1)
                                                                        {
                                                                            break;
                                                                        }
                                                                        int cross_m;
                                                                        if(road_map[m].start_cross==iter_cross_map->first)
                                                                        {
                                                                            cross_m=road_map[m].end_cross;
                                                                        }
                                                                        else
                                                                        {
                                                                            cross_m=road_map[m].start_cross;
                                                                        }
                                                                        int pre_m=road_map[m].road_gesture[cross_m][pre_j][pre_i];/****不是同一条道路！！！**/

                                                                        if(pre_m!=0)
                                                                        {


                                                                        if(car_map[pre_m].is_start==0)/**该车处于等待**/
                                                                        {
                                                                            if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                            {
                                                                                if(car_map[pre_m].direction[0]=='D'||car_map[pre_m].direction[0]=='L')

                                                                                    i= 1; /****表示需要等待**/
                                                                            }
                                                                            Q=1;/**找到第一辆等待的车过后一定要退出当前道路*/
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***有其他直行或者左转的车辆经过***/
                                                    {
                                                        //仍为等待，开始下一条道路！！！！！！！！！！！！！！！！！！
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    if(i==0)
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它:下一位置**/
                                                        if(P.first>=0)/**能进入下一道路**/
                                                        {
                                                            //更新信息！！！！！！，调度下一辆车
                                                            car_map[car_id].road_locate=P;/**更新车辆状态中的pair**/
                                                            car_map[car_id].is_start=1;/**更新为终止状态**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**下一辆车**/
                                                        }
                                                        if(P.first==-1)/**不能进入下一道路**/
                                                        {
                                                            //仍为等待，调度下一道路!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**确定终止，停在当前道路 ***/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**下一辆车**/
                                                        }

                                                    }

                                                }
                                            }
                                            else/**不能出道路**/
                                            {
                                                //更新车辆、道路信息
                                                car_map[car_id].is_start=1;
                                                car_map[car_id].road_locate.second=m-car_map[car_id].speed_now;
                                                road_map[road_id].road_gesture[the_cross_now][n][m]=0;
                                                road_map[road_id].road_gesture[the_cross_now][n][m-car_map[car_id].speed_now]=car_id;
                                                --PM2;
                                                --no_change;
                                                --wait_num;
                                                continue;//遍历该道路下一辆车
                                            }
                                            }
                                            else////////该车前面有阻挡(stock+1)为它的归属
                                            {
                                                car_map[car_id].is_start=1;
                                                car_map[car_id].road_locate.second=stock+1;
                                                road_map[road_id].road_gesture[the_cross_now][n][m]=0;
                                                road_map[road_id].road_gesture[the_cross_now][n][stock+1]=car_id;
                                                --PM2;
                                                --no_change;
                                                --wait_num;
                                                continue;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(wait_num==0)//该路口没有处于等待状态的车辆
                    {
                        break;
                    }
                    if(PM2==0)/***该路口有等待状态车辆，但却没有车辆终止**/
                    {
                        total_wait_num+=wait_num;
                        break; //调度下一个路口
                    }
                }
            }
            if(total_wait_num==0)
            {
                return make_pair(-1,0);
            }

            if(no_change==0)/**出现死锁!!!!!!!!!!!!找到第一个可以改变行驶路线的车辆并返回*/
            {
                cout<<" locked !"<<endl;system("pause");
                //计算出道路中处于等待状态的车辆的数目,并且记录在number_one中
                int number_one =0;
                int the_car_id =0;
                for(map<int ,Road>::iterator iter=road_map.begin();iter!=road_map.end();++iter)
                {
                    for(int i=0;i!=road_map[iter->first].channel_num;++i)
                    {
                        for(int j=0;j!=road_map[iter->first].road_length;++j)
                        {
                            for(map<int ,int **>::iterator pt_cross=road_map[iter->first].road_gesture.begin();pt_cross!=road_map[iter->first].road_gesture.end();++pt_cross)
                            {
                            the_car_id=pt_cross->second[i][j];
                            if(the_car_id!=0)
                            {
                                if(car_map[the_car_id].is_start==0)
                                {
                                    ++number_one;
                                }
                            }

                            }

                        }
                    }
                }
                cout<<"the locked number is : "<<number_one<<endl;
                multimap<int ,pair<int , int> >map_one;
                //升序道路，找到第一辆为等待状态且第二优先级方向可以行驶的车辆，若以其第二方向行驶可以使处于死锁的车的数量减少
                //若以第二优先级都不能使死锁车数目减少，则以第三、第四优先级跑，返回其优先级的值和车辆以及改变其的第几个优先级方向
                int PP=1;
                while(PP)
                {
                for(map<int ,Road>::iterator iter=road_map.begin();iter!=road_map.end();++iter)
                {
                    int Q=0;
                    int road_id=iter->first;
                    for(map<int ,int **>::iterator pt_cross=road_map[road_id].road_gesture.begin();pt_cross!=road_map[road_id].road_gesture.end();++pt_cross)
                    {
                    for(int i=0;i!=road_map[road_id].road_length;++i)
                    {
                        if(Q==1)
                        {
                            break;
                        }

                        for(int j=0;j!=road_map[road_id].channel_num;++j)
                        {
                            if(Q==1)
                            {
                                break;
                            }
                            int car_id=road_map[road_id].road_gesture[pt_cross->first][j][i];
                            if(car_id!=0)//找到一辆车
                            {
                                if(car_map[car_id].is_start==0)//找到第一辆处于等待状态的车
                                {
                                    if(car_map[car_id].direction[PP]!='N')//如果存在第二优先级
                                    {

                                        exchange(car_map_copy[car_id].next_road[0],car_map_copy[car_id].next_road[PP]);
                                        exchange(car_map_copy[car_id].next_cross[0],car_map_copy[car_id].next_cross[PP]);
                                        exchange_char(car_map_copy[car_id].direction[0],car_map_copy[car_id].direction[PP]);

                                        int number =test_lock_num(car_map_copy , road_map_copy , cross_map);

                                        int B=0;
                                        car_map_copy[car_id].road_now=car_map_copy[car_id].next_road[0];
                                        car_map_copy[car_id].cross_now=car_map_copy[car_id].next_cross[0];
                                        renew_road_cross(car_id,car_map_copy,cross_map,road_map_copy);
                                        if(car_map_copy[car_id].next_road[0]==car_map_copy[car_id].road_now)
                                        {
                                            if(car_map_copy[car_id].next_road[1]==0)
                                            {
                                                if(car_map_copy[car_id].next_road[2]==0)
                                                {
                                                    if(car_map_copy[car_id].next_road[3]==0)
                                                    {
                                                        B=1;
                                                    }
                                                }
                                            }
                                        }
                                        if(number<number_one&&B!=1)//满足条件则入map
                                        {
                                            //这里还需要加一个条件 也就是使下一次调度能保证PP！=4
                                            map_one.insert(make_pair(number,make_pair(PP,car_id)));//map_one中存放的是改变各车的

                                        }
                                        copy_car_map(car_map_copy_one ,car_map_copy);
                                        copy_road_map(road_map_copy_one ,road_map_copy);
                                    }


                                        Q=1;

                                }
                            }
                        }
                    }

                    }

                }
                ++PP;
                if(PP==4)//若果等于 找到一辆车改变其行驶方向并且能够确定终止   此时返回
                {
                    break;
                }
                }
                if(!map_one.empty())//返回使死锁车数目减少最多的车辆信息
                {
                    multimap<int ,pair<int , int> >::iterator iter_one=map_one.begin();
                    cout<<"the smallest number is :"<<iter_one->first<<endl;
                    return iter_one->second;
                }
                /****如果出现减不下来并且高不上去的情况应该怎么办*******/
                else//如果为空 那么改变所有车辆的信息都不能使车辆信息减少
                {
                    cout<<"the lock number cannot be smaller happened !"<<endl;
                    system("pause");
                    return make_pair(10000,0);
                       //first为10000表示特殊情况出现，应该特殊处理
                }
        }
}
}













