#include"car_road_cross.h"
using namespace std;
void exchange_char(char &a, char &b)
{
    char c=a;
    a=b;
    b=c;
}
void exchange(int &a,int &b)/**********������������ֵ************/
{
    int temp=a;
    a=b;
    b=temp;
}
/*******************����ÿ��·�ڵ���ڵ�·�������ͨ·��******************/
void set_cross(map<int,Cross>& cross_map,map<int,Road>& road_map)
{
    map<int,Cross>::iterator pt_cross_map=cross_map.begin();
    while(pt_cross_map!=cross_map.end())
    {
        int cross_id=pt_cross_map->first; //·��ID
        for(int i=0;i!=rand_size;++i)  /******************ȷ����ͨ·��*****************/
        {
            int road_id=cross_map[cross_id].road[i];//��·ID
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
        for(int i=rand_size-1;i!=0;--i)  /********����·�ڵĸ�����·***********/
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
/**********************************************����car��·����Ϣ������map******************************/
int make_car_multimap(const char *direction,map<int, multimap<int ,Car > >&car_multimap)
{
    ifstream infile(direction);
    if(!infile)
    {
        cerr<<"error: unable to open car file:"<<direction<<endl;
        return -1;
    }
    string line; Car car;char sh;
    int car_number=0;/**�����ܵ�����**/
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
/*********************�����������·��****************************/
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
/************************************����cross����Ϣ������map*************************************************/
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
/************************************����road����Ϣ������map*************************************************/
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

        road_map[road.road_ID].road_gesture[road.start_cross]=(int **)new int* [road.channel_num];/***������start����crossΪ��ʼ·�ڵĿ�λ**/
        for(int i=0;i<road.channel_num;++i)
            road_map[road.road_ID].road_gesture[road.start_cross][i]=new int[road.road_length];
        for(int i=0;i<road.channel_num;++i)
        {
            for(int j=0;j<road.road_length;++j)
                 road_map[road.road_ID].road_gesture[road.start_cross][i][j]=0;  /***��ʼ����·��Ϣ����·�ϵ�����λ���޳�������Ϊ0***/

        }
        if(road.isduplex==1)/***������end����crossΪ��ʼ·�ڵĿ�λ**/
        {
            road_map[road.road_ID].road_gesture[road.end_cross]=(int **)new int* [road.channel_num];
            for(int i=0;i<road.channel_num;++i)
                road_map[road.road_ID].road_gesture[road.end_cross][i]=new int[road.road_length];
            for(int i=0;i<road.channel_num;++i)
            {
                for(int j=0;j<road.road_length;++j)
                    road_map[road.road_ID].road_gesture[road.end_cross][i][j]=0;  /***��ʼ����·��Ϣ����·�ϵ�����λ���޳�������Ϊ0***/
            }

        }
    }
    infile.close();
    return number;
}
/***************************����ʼ·�ں���ֹ·�ڣ�������ʼ����ֹ֮�����ֵ̽���Ŀ**********************************/
int test_distance(int cross_begin,int cross_end,map<int ,Cross>&cross_map)/***********!!!!!***********/
{
    int Logo=1,distance=0;
    if(cross_begin!=cross_end)/**��·�ڲ�Ϊ��ֹ·��**/
    {
        list<int> list_one,list_two;
        set<int> sec;/**��ű������˵�cross**/
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
/*******************************��cross_now���յ�crossȷ��next_cross[]��next_road[]*******************************************/
void renew_road_cross(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map,map<int,Road>& road_map)
{
    int begins=car_map[car_id].cross_now;
    int ends=car_map[car_id].end_cross;
    int cross_band[rand_size]; /**��һ��·�ڵ����ȼ�����**/
    for(int i=0;i!=rand_size;++i)  /*********cross_band�����ڽ�·�ڵ��յ�ľ��룬���ڽ�·��ʱΪ0*********/
    {
        if(cross_map[begins].other_cross[i]>0)
            cross_band[i]=test_distance(cross_map[begins].other_cross[i],ends,cross_map);
        else
            cross_band[i]=-1;
    }
    for(int j=0;j!=rand_size;++j)/*********��cross_band�õ��������ȼ�����һ·������cross_bands************/
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
            car_map[car_id].next_cross[j]=0;    /**************0��ʾû��·��*****************/
        else
        {
            car_map[car_id].next_cross[j]=cross_map[begins].other_cross[Min];
            cross_band[Min]=-1;
        }
    }
   /**ȷ��next_cross***/
    for(int i=0;i!=rand_size;++i)
    {
        int temp=car_map[car_id].next_cross[i];
        if(temp==0)/***û��·����û�е�·*****/
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
/*****ת��������ݳ�����ǰ����·�ں���һ��·�ó�****/
void renew_direction(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map)/******���¸ó��ڸ�·�ڵ�ת�����********/
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
/*******���³���********/
void renew_speed_now(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map)
{
    int temp=car_map[car_id].road_now;
    car_map[car_id].speed_now=car_map[car_id].max_speed;
    if(car_map[car_id].max_speed>road_map[temp].max_speed)
        car_map[car_id].speed_now=road_map[temp].max_speed;
}
pair<int, int> next_location(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map)/***first: -2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ��������ȷ����һλ��**/
{
    int road_next=car_map[car_id].next_road[0];
    int the_cross_now=car_map[car_id].cross_now;
    int k=road_map[road_next].max_speed-car_map[car_id].road_locate.second;/**kΪ��һ��·����ʻ�ľ���*/
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
                if(car_map[the_car].is_start==0)/***��Ϊ�ȴ�����Ϊ�ȴ�**/
                {
                    return make_pair(-1,0);
                }

                if(car_map[the_car].is_start==1)/**��Ϊ��ֹ***/
                {
                    if(j==length)
                    {
                        if(i==road_map[road_next].channel_num-1)
                        {
                            return make_pair(-2,0);
                        }
                        else
                        {
                            break; /**������һ����**/
                        }

                    }
                    else
                    {
                        return make_pair(i,j+1);
                    }
                }
            }
        }
        if(j==length-k)/**�޳��谭**/
        {
            return make_pair(i,j+1);
        }
    }
}

void copy_car_map(map<int ,Car>&real_one , map<int ,Car>&copy_one)  /**���Ƶ�ͼ**/
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

        copy_one[iter->first].road_gesture[iter->second.start_cross]=(int **)new int* [iter->second.channel_num];/**����start_cross**/
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
        copy_one[iter->first].road_gesture[iter->second.end_cross]=(int **)new int* [iter->second.channel_num];/**����start_cross**/
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

int test_lock_num(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map)//��һ�����ȣ����صȴ���������Ŀ
{
        /**�����ڵȴ��ĳ���**/
        /**��ֹһ����һ��**/
        while(1)
        {
            int no_change=0;
            int total_wait_num=0;
            for(map<int ,Cross>::iterator iter_cross_map=cross_map.begin();iter_cross_map!=cross_map.end();++iter_cross_map)/**������ȸ���·��**/
            {
                //int cross_id=iter_cross_map->first;
                //int goto_next_cross=0;/**����Ƿ�������һ·��**/
                while(1)/**���ѭ�����ȸ�·�ڵ�·ֱ����·�����г�����Ϊ��ֹ״̬����ѭ��һ��û�г�����Ϊ��ֹ״̬**/
                {
                    int PM2 =0;
                    int wait_num=0;/**����һ��·�ڳ��Ӵ��ڵȴ�״̬������**/
                    for(int i=0;i!=rand_size;++i)/**���ȸ�·�ڸ�����·�ڵ�·**/
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
                            for(int m=0;m<road_map[road_id].road_length;++m)/**˳����ȵ�·��������**/
                            {
                                if(goto_next_road)
                                {
                                    break;
                                }
                                for(int n=0;n<road_map[road_id].channel_num;++n)
                                {
                                    if(road_map[road_id].road_gesture[the_cross_now][n][m]!=0)/**�ҵ���һ����**/
                                    {
                                        int car_id=road_map[road_id].road_gesture[the_cross_now][n][m];
                                        if(car_map[car_id].is_start==0) /***�Ҹó����ڵȴ�״̬(����ȷ���ó�ǰ��һ�����赲??????????)**/
                                        {
                                            ++wait_num;
                                            int stock;////�жϸó�ǰ���Ƿ�����赲
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
                                            if(stock==-1)/**�Ҹó�ǰ�����赲������������������*/
                                            {



                                            if(car_map[car_id].speed_now>m)/**�ܳ���·**/
                                            {
                                                if(car_map[car_id].cross_now==car_map[car_id].end_cross)/**·��Ϊ��ֹ·��**/
                                                {
                                                    //������Ϣ��������һ����
                                                    car_map[car_id].is_start= 2;   /**�����յ㣡����������������������������**/
                                                    road_map[road_id].road_gesture[the_cross_now][n][m]= 0;/**���µ�·״̬**/
                                                    --no_change;
                                                    --PM2;
                                                    --wait_num;
                                                    continue;   /**������һ���� **/
                                                }
                                                //��Ϊ��ֹ·�ڣ����ж�����תֱ�����ȼ�
                                                if(car_map[car_id].direction[0]=='D')/**ֱ��*/
                                                {
                                                    pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ����:��һλ��**/
                                                    if(P.first>=0)/**�ܽ�����һ��·**/
                                                    {
                                                        //������Ϣ��������������������һ����
                                                        car_map[car_id].road_locate=P;/**���³���״̬�е�pair**/
                                                        car_map[car_id].is_start=1;/**����Ϊ��ֹ״̬**/
                                                        car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                        car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                        renew_speed_now(car_id,car_map,road_map);
                                                        renew_road_cross(car_id,car_map,cross_map,road_map);
                                                        renew_direction(car_id,car_map,cross_map);
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                        road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue; /**��һ����**/
                                                    }
                                                    if(P.first==-1)/**���ܽ�����һ��·**/
                                                    {
                                                        //��Ϊ�ȴ���������һ��·!
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    if(P.first==-2)/**ȷ����ֹ��ͣ�ڵ�ǰ��· **/
                                                    {
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                        road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                        car_map[car_id].is_start=1;
                                                        car_map[car_id].road_locate.second=0;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue;/**��һ����**/
                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='L')/**��ת*/
                                                {
                                                    //�жϸò������ߣ������ã���Ϊ�ȴ�����ʼ��һ��·
                                                    //�������ߣ��ж��ܷ������һ��·
                                                    //�ܽ�����һ��·��������Ϣ������������������һ����
                                                    //���ܽ�����һ��·,��Ϊ�ȴ�
                                                    int i=0;/**��i��ֵ��ȷ���Ƿ���Ҫ�ȴ� 1���ȴ� 0�����õȴ�**/
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


                                                                        if(car_map[pre_m].is_start==0)/**�ó����ڵȴ�**/
                                                                        {
                                                                            if(car_map[pre_m].direction[0]=='D')
                                                                            {
                                                                                if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                                    i= 1; /**��ʾ��Ҫ�ȴ�**/
                                                                            }
                                                                            Q=1;
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***������ֱ�еĳ�������***/
                                                    {
                                                        goto_next_road=1;//��Ϊ�ȴ�����ʼ��һ����·������������������������������������
                                                        break;
                                                    }
                                                    if(i==0)
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ����:��һλ��**/
                                                        if(P.first>=0)/**�ܽ�����һ��·**/
                                                        {
                                                            //������Ϣ��������������������һ����
                                                            car_map[car_id].road_locate=P;/**���³���״̬�е�pair**/
                                                            car_map[car_id].is_start=1;/**����Ϊ��ֹ״̬**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**��һ����**/
                                                        }
                                                        if(P.first==-1)/**���ܽ�����һ��·**/
                                                        {
                                                            //��Ϊ�ȴ���������һ��·!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**ȷ����ֹ��ͣ�ڵ�ǰ��· **/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**��һ����**/
                                                        }

                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='R')/**��ת**/
                                                {
                                                    //�жϸò������ߣ������ã���Ϊ�ȴ�����ʼ��һ��·
                                                    //�������ߣ��ж��ܷ������һ��·
                                                    //�ܽ�����һ��·��������Ϣ������������������һ����
                                                    //���ܽ�����һ��·,��Ϊ�ȴ�
                                                    int i=0;/**��i��ֵ��ȷ���Ƿ���Ҫ�ȴ� 1���ȴ� 0�����õȴ�**/
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


                                                                        if(car_map[pre_m].is_start==0)/**�ó����ڵȴ�**/
                                                                        {
                                                                            if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                            {
                                                                                if(car_map[pre_m].direction[0]=='D'||car_map[pre_m].direction[0]=='L')

                                                                                    i= 1; /****��ʾ��Ҫ�ȴ�**/
                                                                            }
                                                                            Q=1;
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***������ֱ�л�����ת�ĳ�������***/
                                                    {
                                                        //��Ϊ�ȴ�����ʼ��һ����·������������������������������������
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ����:��һλ��**/
                                                        if(P.first>=0)/**�ܽ�����һ��·**/
                                                        {
                                                            //������Ϣ��������������������һ����
                                                            car_map[car_id].road_locate=P;/**���³���״̬�е�pair**/
                                                            car_map[car_id].is_start=1;/**����Ϊ��ֹ״̬**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**��һ����**/
                                                        }
                                                        if(P.first==-1)/**���ܽ�����һ��·**/
                                                        {
                                                            //��Ϊ�ȴ���������һ��·!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**ȷ����ֹ��ͣ�ڵ�ǰ��· ***/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**��һ����**/
                                                        }

                                                    }

                                                }
                                            }
                                            else/**���ܳ���·**/
                                            {
                                                //���³�������·��Ϣ
                                                car_map[car_id].is_start=1;
                                                car_map[car_id].road_locate.second=m-car_map[car_id].speed_now;
                                                road_map[road_id].road_gesture[the_cross_now][n][m]=0;
                                                road_map[road_id].road_gesture[the_cross_now][n][m-car_map[car_id].speed_now]=car_id;
                                                --PM2;
                                                --no_change;
                                                --wait_num;
                                                continue;//�����õ�·��һ����
                                            }
                                            }
                                            else////////�ó�ǰ�����赲(stock+1)Ϊ���Ĺ���
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
                    if(PM2==0)/***�еȴ�״̬���޳���ֹ**/
                    {
                        total_wait_num+=wait_num;
                        break; //������һ��·��
                    }
                }
            }

            if(total_wait_num==0)
            {
                return 0;
            }
            if(no_change==0)/**�������������ظõ�ͼĿǰ���ڵȴ�״̬�ĳ�������Ŀ*/
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


/**�����Ƿ�����first>=1��ʾ������������secondΪ���Խ�������ĳ���id��firstΪ0��ʾ�����������**/
pair<int ,int > test_lock(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map)
{
        map<int ,Car> car_map_copy ,car_map_copy_one;
        map<int ,Road> road_map_copy, road_map_copy_one;
        copy_car_map(car_map , car_map_copy);
        copy_car_map(car_map , car_map_copy_one);
        copy_road_map(road_map , road_map_copy);
        copy_road_map(road_map , road_map_copy_one);
        /**�����ڵȴ��ĳ���**/
        /**��ֹһ����һ��**/
        while(1)
        {
            int no_change=0;
            int total_wait_num=0;
            for(map<int ,Cross>::iterator iter_cross_map=cross_map.begin();iter_cross_map!=cross_map.end();++iter_cross_map)/**������ȸ���·��**/
            {
                //int cross_id=iter_cross_map->first;
                //int goto_next_cross=0;/**����Ƿ�������һ·��**/

                while(1)/**���ѭ�����ȸ�·�ڵ�·ֱ����·�����г�����Ϊ��ֹ״̬����ѭ��һ��û�г�����Ϊ��ֹ״̬**/
                {
                    int PM2=0;
                    int wait_num=0;/**����һ��·�ڳ��Ӵ��ڵȴ�״̬������**/
                    for(int i=0;i!=rand_size;++i)/**���ȸ�·�ڸ�����·�ڵ�·**/
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
                            for(int m=0;m<road_map[road_id].road_length;++m)/**˳����ȵ�·��������**/
                            {
                                if(goto_next_road==1)
                                {
                                    break;
                                }
                                for(int n=0;n<road_map[road_id].channel_num;++n)
                                {
                                    if(road_map[road_id].road_gesture[the_cross_now][n][m]!=0)/**�ҵ���һ����**/
                                    {
                                        int car_id=road_map[road_id].road_gesture[the_cross_now][n][m];

                                        if(car_map[car_id].is_start==0) /***�Ҹó����ڵȴ�״̬(����ȷ���ó�ǰ��һ�����赲??????????)**/
                                        {
                                            ++wait_num;
                                            int stock;////�жϸó�ǰ���Ƿ�����赲
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
                                            if(stock==-1)/**�Ҹó�ǰ�����赲������������������*/
                                            {
                                            if(car_map[car_id].speed_now>m)/**�ܳ���·**/
                                            {
                                                if(car_map[car_id].cross_now==car_map[car_id].end_cross)/**·��Ϊ��ֹ·��**/
                                                {
                                                    //������Ϣ��������һ����
                                                    car_map[car_id].is_start= 2;   /**�����յ㣡����������������������������**/
                                                    road_map[road_id].road_gesture[the_cross_now][n][m]= 0;/**���µ�·״̬**/
                                                    --no_change;
                                                    --PM2;
                                                    --wait_num;
                                                    continue;   /**������һ���� **/
                                                }
                                                //��Ϊ��ֹ·�ڣ����ж�����תֱ�����ȼ�
                                                if(car_map[car_id].direction[0]=='D')/**ֱ��*/
                                                {
                                                    pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ����:��һλ��**/
                                                    if(P.first>=0)/**�ܽ�����һ��·**/
                                                    {
                                                        //������Ϣ��������������������һ����
                                                        car_map[car_id].road_locate=P;/**���³���״̬�е�pair**/
                                                        car_map[car_id].is_start=1;/**����Ϊ��ֹ״̬**/
                                                        car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                        car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                        renew_speed_now(car_id,car_map,road_map);
                                                        renew_road_cross(car_id,car_map,cross_map,road_map);
                                                        renew_direction(car_id,car_map,cross_map);
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                        road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue; /**��һ����**/
                                                    }
                                                    if(P.first==-1)/**���ܽ�����һ��·**/
                                                    {
                                                        //��Ϊ�ȴ���������һ��·!
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    if(P.first==-2)/**ȷ����ֹ��ͣ�ڵ�ǰ��· **/
                                                    {
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                        road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                        car_map[car_id].is_start=1;
                                                        car_map[car_id].road_locate.second=0;
                                                        --no_change;
                                                        --PM2;
                                                        --wait_num;
                                                        continue;/**��һ����**/
                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='L')/**��ת*/
                                                {
                                                    //�жϸò������ߣ������ã���Ϊ�ȴ�����ʼ��һ��·
                                                    //�������ߣ��ж��ܷ������һ��·
                                                    //�ܽ�����һ��·��������Ϣ������������������һ����
                                                    //���ܽ�����һ��·,��Ϊ�ȴ�
                                                    int i=0;/**��i��ֵ��ȷ���Ƿ���Ҫ�ȴ� 1���ȴ� 0�����õȴ�**/
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
                                                                        if(car_map[pre_m].is_start==0)/**�ó����ڵȴ�**/
                                                                        {
                                                                            if(car_map[pre_m].direction[0]=='D')
                                                                            {
                                                                                if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                                    i= 1; /**��ʾ��Ҫ�ȴ�**/
                                                                            }
                                                                            Q=1;
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***������ֱ�еĳ�������***/
                                                    {
                                                        goto_next_road=1;//��Ϊ�ȴ�����ʼ��һ����·������������������������������������
                                                        break;
                                                    }
                                                    if(i==0)
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ����:��һλ��**/
                                                        if(P.first>=0)/**�ܽ�����һ��·**/
                                                        {
                                                            //������Ϣ��������������������һ����
                                                            car_map[car_id].road_locate=P;/**���³���״̬�е�pair**/
                                                            car_map[car_id].is_start=1;/**����Ϊ��ֹ״̬**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**��һ����**/
                                                        }
                                                        if(P.first==-1)/**���ܽ�����һ��·**/
                                                        {
                                                            //��Ϊ�ȴ���������һ��·!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**ȷ����ֹ��ͣ�ڵ�ǰ��· **/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**��һ����**/
                                                        }

                                                    }
                                                }
                                                if(car_map[car_id].direction[0]=='R')/**��ת**/
                                                {
                                                    //�жϸò������ߣ������ã���Ϊ�ȴ�����ʼ��һ��·
                                                    //�������ߣ��ж��ܷ������һ��·
                                                    //�ܽ�����һ��·��������Ϣ������������������һ����
                                                    //���ܽ�����һ��·,��Ϊ�ȴ�
                                                    int i=0;/**��i��ֵ��ȷ���Ƿ���Ҫ�ȴ� 1���ȴ� 0�����õȴ�**/
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
                                                                        int pre_m=road_map[m].road_gesture[cross_m][pre_j][pre_i];/****����ͬһ����·������**/

                                                                        if(pre_m!=0)
                                                                        {


                                                                        if(car_map[pre_m].is_start==0)/**�ó����ڵȴ�**/
                                                                        {
                                                                            if(car_map[pre_m].next_road[0]==car_map[car_id].next_road[0])
                                                                            {
                                                                                if(car_map[pre_m].direction[0]=='D'||car_map[pre_m].direction[0]=='L')

                                                                                    i= 1; /****��ʾ��Ҫ�ȴ�**/
                                                                            }
                                                                            Q=1;/**�ҵ���һ���ȴ��ĳ�����һ��Ҫ�˳���ǰ��·*/
                                                                        }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    if(i==1)/***������ֱ�л�����ת�ĳ�������***/
                                                    {
                                                        //��Ϊ�ȴ�����ʼ��һ����·������������������������������������
                                                        goto_next_road=1;
                                                        break;
                                                    }
                                                    if(i==0)
                                                    {
                                                        pair<int, int> P =next_location(car_id,car_map,road_map);/**first:-2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ����:��һλ��**/
                                                        if(P.first>=0)/**�ܽ�����һ��·**/
                                                        {
                                                            //������Ϣ��������������������һ����
                                                            car_map[car_id].road_locate=P;/**���³���״̬�е�pair**/
                                                            car_map[car_id].is_start=1;/**����Ϊ��ֹ״̬**/
                                                            car_map[car_id].road_now=car_map[car_id].next_road[0];
                                                            car_map[car_id].cross_now=car_map[car_id].next_cross[0];
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue; /**��һ����**/
                                                        }
                                                        if(P.first==-1)/**���ܽ�����һ��·**/
                                                        {
                                                            //��Ϊ�ȴ���������һ��·!
                                                            goto_next_road=1;
                                                            break;

                                                        }
                                                        if(P.first==-2)/**ȷ����ֹ��ͣ�ڵ�ǰ��· ***/
                                                        {
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;
                                                            road_map[road_id].road_gesture[the_cross_now][n][0]= car_id;
                                                            car_map[car_id].is_start=1;
                                                            car_map[car_id].road_locate.second=0;
                                                            --no_change;
                                                            --PM2;
                                                            --wait_num;
                                                            continue;/**��һ����**/
                                                        }

                                                    }

                                                }
                                            }
                                            else/**���ܳ���·**/
                                            {
                                                //���³�������·��Ϣ
                                                car_map[car_id].is_start=1;
                                                car_map[car_id].road_locate.second=m-car_map[car_id].speed_now;
                                                road_map[road_id].road_gesture[the_cross_now][n][m]=0;
                                                road_map[road_id].road_gesture[the_cross_now][n][m-car_map[car_id].speed_now]=car_id;
                                                --PM2;
                                                --no_change;
                                                --wait_num;
                                                continue;//�����õ�·��һ����
                                            }
                                            }
                                            else////////�ó�ǰ�����赲(stock+1)Ϊ���Ĺ���
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
                    if(wait_num==0)//��·��û�д��ڵȴ�״̬�ĳ���
                    {
                        break;
                    }
                    if(PM2==0)/***��·���еȴ�״̬��������ȴû�г�����ֹ**/
                    {
                        total_wait_num+=wait_num;
                        break; //������һ��·��
                    }
                }
            }
            if(total_wait_num==0)
            {
                return make_pair(-1,0);
            }

            if(no_change==0)/**��������!!!!!!!!!!!!�ҵ���һ�����Ըı���ʻ·�ߵĳ���������*/
            {
                cout<<" locked !"<<endl;system("pause");
                //�������·�д��ڵȴ�״̬�ĳ�������Ŀ,���Ҽ�¼��number_one��
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
                //�����·���ҵ���һ��Ϊ�ȴ�״̬�ҵڶ����ȼ����������ʻ�ĳ�����������ڶ�������ʻ����ʹ���������ĳ�����������
                //���Եڶ����ȼ�������ʹ��������Ŀ���٣����Ե������������ȼ��ܣ����������ȼ���ֵ�ͳ����Լ��ı���ĵڼ������ȼ�����
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
                            if(car_id!=0)//�ҵ�һ����
                            {
                                if(car_map[car_id].is_start==0)//�ҵ���һ�����ڵȴ�״̬�ĳ�
                                {
                                    if(car_map[car_id].direction[PP]!='N')//������ڵڶ����ȼ�
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
                                        if(number<number_one&&B!=1)//������������map
                                        {
                                            //���ﻹ��Ҫ��һ������ Ҳ����ʹ��һ�ε����ܱ�֤PP��=4
                                            map_one.insert(make_pair(number,make_pair(PP,car_id)));//map_one�д�ŵ��Ǹı������

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
                if(PP==4)//�������� �ҵ�һ�����ı�����ʻ�������ܹ�ȷ����ֹ   ��ʱ����
                {
                    break;
                }
                }
                if(!map_one.empty())//����ʹ��������Ŀ�������ĳ�����Ϣ
                {
                    multimap<int ,pair<int , int> >::iterator iter_one=map_one.begin();
                    cout<<"the smallest number is :"<<iter_one->first<<endl;
                    return iter_one->second;
                }
                /****������ּ����������Ҹ߲���ȥ�����Ӧ����ô��*******/
                else//���Ϊ�� ��ô�ı����г�������Ϣ������ʹ������Ϣ����
                {
                    cout<<"the lock number cannot be smaller happened !"<<endl;
                    system("pause");
                    return make_pair(10000,0);
                       //firstΪ10000��ʾ����������֣�Ӧ�����⴦��
                }
        }
}
}













