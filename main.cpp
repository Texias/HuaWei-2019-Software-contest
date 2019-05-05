#include"car_road_cross.h"
using namespace std;

int main(/*int argc,char **argv*/)
{
    const char *argv[5];
    argv[0]="hellow world!";
    argv[1]="F:\\Codeblocks\\Huawei\\huawei_contest\\SDK\\SDK_C++\\config_11\\car.txt";
    argv[2]="F:\\Codeblocks\\Huawei\\huawei_contest\\SDK\\SDK_C++\\config_11\\road.txt";
    argv[3]="F:\\Codeblocks\\Huawei\\huawei_contest\\SDK\\SDK_C++\\config_11\\cross.txt";
    argv[4]="F:\\Codeblocks\\Huawei\\huawei_contest\\SDK\\SDK_C++\\config_11\\answer.txt";

    map<int, multimap<int, Car> >car_multimap;

    map<int, Road> road_map;
    map<int, Cross> cross_map;
    map<int, list<int> > The_answer_map;

    int Total_car_num_real=make_car_multimap(argv[1],car_multimap);/***车子总的数量,到达终点一个减一个，减为零时终止**/

    int Total_road_location=make_road_map(argv[2],road_map);       /**道路中车辆总的坑位*/


    int car_num_restriction =static_cast<int>(Total_road_location/4); /**（地图中车辆数目的限制）这个程序最关键的地方！！！！！！！！！*/

    make_cross_map(argv[3],cross_map);
    set_cross(cross_map,road_map);       /******确定路口状态信息*******/
    int time_to_break=0;
    while(1)
    {
    if(time_to_break==1)
    {
        break;
    }
    map<int, Car> car_map;
    map<int, Car> car_map_ready;
    int car_map_number=0;
    int car_map_ready_number=0;
    int Total_car_num=Total_car_num_real;
    int time_pierce=1;/**时间片**/
    int goto_next_round=0;
    int timepiere_car_num=0;

    car_num_restriction=1000;
    while(time_pierce)/***开始调度***/
    {
        cout<<"the total car_num is :"<<Total_car_num<<endl;

        cout<<"the car_num_restrition is :"<<car_num_restriction<<endl;

        cout<<"the car on the road is : "<<timepiere_car_num<<endl;
        if(Total_car_num==0)
        {
            time_to_break=1;
            break;
        }
       //在一个调度完成后道路中车辆的数目，用以确定是否应该 发车

        /**处理在道路中的车辆**/
        for(map<int ,Road>::iterator iter_road_map=road_map.begin();iter_road_map!=road_map.end();++iter_road_map)/**遍历每条道路***/
        {
            for(int i=0;i!=iter_road_map->second.road_length;++i)/***遍历一条道路的所有车辆并且确定状态**/
            {
                for(int j=0;j!=iter_road_map->second.channel_num;++j)
                {
                    for(map<int ,int **>::iterator pt_cross=road_map[iter_road_map->first].road_gesture.begin();pt_cross!=road_map[iter_road_map->first].road_gesture.end();++pt_cross)
                    {
                    if(iter_road_map->second.road_gesture[pt_cross->first][j][i]!=0)
                    {

                        int car_id=iter_road_map->second.road_gesture[pt_cross->first][j][i];

                        int m;
                        for(m=i-1;m!=-1;--m)
                        {
                            if(iter_road_map->second.road_gesture[pt_cross->first][j][m]==0)
                            {
                                continue;
                            }
                            if(i-m<=car_map[car_id].speed_now)/**前方有车，并且会阻挡**/
                            {
                                int pre_car_id=iter_road_map->second.road_gesture[pt_cross->first][j][m]; /**阻挡车的ID**/
                                if(car_map[pre_car_id].is_start==0)/**前车为等待，则该车也为等待**/
                                {
                                    car_map[car_id].is_start=0; /**该车也为等待**/
                                    break;
                                }
                                if(car_map[pre_car_id].is_start==1)/**前车为终止**/
                                {
                                    car_map[car_id].is_start=1;/**该车也为终止**/
                                    car_map[car_id].road_locate.second= m+1;/**更新车辆状态**/
                                    //更新道路状态
                                    iter_road_map->second.road_gesture[pt_cross->first][j][i]=0;
                                    iter_road_map->second.road_gesture[pt_cross->first][j][m+1]=car_id;

                                    break;
                                }
                            }
                            if(i-m>car_map[car_id].speed_now)/**前方有车，但不会阻挡**/
                            {
                                car_map[car_id].is_start=1;/**该车也为终止**/
                                car_map[car_id].road_locate.second=i-car_map[car_id].speed_now;/**更新车辆状态**/
                                //更新道路状态
                                iter_road_map->second.road_gesture[pt_cross->first][j][i]=0;
                                iter_road_map->second.road_gesture[pt_cross->first][j][i-car_map[car_id].speed_now]=car_id;

                                break;
                            }
                        }
                        if(m==-1)/**前方无车**/
                        {
                            if(car_map[car_id].speed_now>i)/**要过路口**/
                            {
                                car_map[car_id].is_start=0;/**该车为等待**/
                                continue;
                            }
                            else/**不过路口**/
                            {
                                //更新道路信息和车辆状态
                                car_map[car_id].is_start=1;/**该车为终止**/
                                car_map[car_id].road_locate.second=i-car_map[car_id].speed_now;/**更新车辆状态**/
                                iter_road_map->second.road_gesture[pt_cross->first][j][i]=0;
                                iter_road_map->second.road_gesture[pt_cross->first][j][i-car_map[car_id].speed_now]=car_id;

                                continue;
                            }
                        }
                    }
                    }

                }
            }
        }
        //cout<<"timepiere :"<<time_pierce<<" can hold the car on the road !"<<endl;
        /**处理在等待的车辆**/
        map<int, Car> copy_car;
        map<int, Road> copy_road;
        copy_car_map(car_map , copy_car);
        copy_road_map(road_map , copy_road);

        //copy_cross_map(cross_map , copy_cross);/////其实cross不太需要
        while(1)  /////////确保这次调度不会出现死锁
        {
            copy_car_map(car_map , copy_car);
            copy_road_map(road_map , copy_road);
            pair<int ,int> will_lock =test_lock(copy_car ,copy_road ,cross_map);

            if(will_lock.first==-1)
            {
                break;
            }

            if(will_lock.first!=-1)
            {
                if(will_lock.first==10000)/**特殊值出现，这个时候复制所有地图 改变道路车辆限制 ，再来一遍，确保一定能出结果**********/
                {
                    goto_next_round =1;
                    break;
                }
                exchange(car_map[will_lock.second].next_road[0],car_map[will_lock.second].next_road[will_lock.first]);
                exchange(car_map[will_lock.second].next_cross[0],car_map[will_lock.second].next_cross[will_lock.first]);
                exchange_char(car_map[will_lock.second].direction[0],car_map[will_lock.second].direction[will_lock.first]);

            }
        }

        if(goto_next_round==1)
        {
            break;
        }
        ////////////一定不会出现死锁
        while(1)
        {

            int no_change=0;/**终止一个减一个**/
            int total_wait_num=0;
            for(map<int ,Cross>::iterator iter_cross_map=cross_map.begin();iter_cross_map!=cross_map.end();++iter_cross_map)/**升序调度各个路口**/
            {
                //int cross_id=iter_cross_map->first;
                //int goto_next_cross=0;/**检测是否变更到下一路口**/
                while(1)/**多次循环调度该路口道路直至该路口所有车辆都为终止状态或者循环一遍没有车辆变为终止状态**/
                {

                    int PM2 =0;
                    int wait_num=0;/**一个路口车子处于等待状态的数量**/
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
                                        if(car_map[car_id].is_start==0) /***且该车处于等待状态**/
                                        {

                                            ++wait_num;
                                            int stock;////判断该车前进是否会有阻挡
                                            for(stock =m-1;stock!=-1;--stock)
                                            {
                                                int the_car_id =road_map[road_id].road_gesture[the_cross_now][n][stock];
                                                if(the_car_id!=0)
                                                {
                                                    int distance=m-stock;
                                                    if(distance>car_map[car_id].max_speed)
                                                    {
                                                        stock=-1;
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        break;
                                                    }
                                                }
                                            }
                                            if(stock==-1)/**且该车前进无阻挡！！！！！！！！！*/
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
                                                    --Total_car_num;
                                                    --timepiere_car_num;
                                                    --wait_num;
                                                    car_map.erase(car_id);
                                                    --car_map_number;
                                                    continue;   /**调度下一辆车 **/
                                                }
                                                //cout<<"car_id : "<<car_id<<" direction[0] : "<<car_map[car_id].gesture.direction[0]<<endl;
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
                                                        The_answer_map[car_id].push_back(car_map[car_id].road_now); //更新所经道路
                                                        renew_speed_now(car_id,car_map,road_map);
                                                        renew_road_cross(car_id,car_map,cross_map,road_map);
                                                        renew_direction(car_id,car_map,cross_map);
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                        road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                        int PV=0;
                                                        while(car_map[car_id].direction[PV]=='N')/****第一优先级为N则按第二优先级跑，知道该优先级不为N***/
                                                        {
                                                           ++PV;
                                                        }
                                                        if(PV!=0)
                                                        {
                                                            exchange(car_map[car_id].next_road[0],car_map[car_id].next_road[PV]);
                                                            exchange(car_map[car_id].next_cross[0],car_map[car_id].next_cross[PV]);
                                                            exchange_char(car_map[car_id].direction[0],car_map[car_id].direction[PV]);
                                                        }
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
                                                            The_answer_map[car_id].push_back(car_map[car_id].road_now); //更新所经道路
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            int PV=0;
                                                            while(car_map[car_id].direction[PV]=='N')/****第一优先级为N则按第二优先级跑，知道该优先级不为N***/
                                                            {
                                                                ++PV;
                                                            }
                                                            if(PV!=0)
                                                                {
                                                                    exchange(car_map[car_id].next_road[0],car_map[car_id].next_road[PV]);
                                                                    exchange(car_map[car_id].next_cross[0],car_map[car_id].next_cross[PV]);
                                                                    exchange_char(car_map[car_id].direction[0],car_map[car_id].direction[PV]);
                                                                }
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
                                                            The_answer_map[car_id].push_back(car_map[car_id].road_now); //更新所经道路
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//更新道路状态
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            int PV=0;
                                                            while(car_map[car_id].direction[PV]=='N')/****第一优先级为N则按第二优先级跑，直到该优先级不为N***/
                                                            {
                                                                ++PV;
                                                            }
                                                            if(PV!=0)
                                                            {
                                                                exchange(car_map[car_id].next_road[0],car_map[car_id].next_road[PV]);
                                                                exchange(car_map[car_id].next_cross[0],car_map[car_id].next_cross[PV]);
                                                                exchange_char(car_map[car_id].direction[0],car_map[car_id].direction[PV]);
                                                            }
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
                                                --no_change;
                                                --PM2;
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
                    if(PM2==0)/***该路口没有处于等待状态的车辆或者所有第一优先级车辆都处于等待状态**/
                    {
                        total_wait_num+=wait_num;
                        break;
                    }
                }
            }

            if(total_wait_num==0)
            {
                break;
            }
            if(no_change==0)
            {
                cout<<" lock again !!!"<<endl;
                system("pause");
            }
        }
        //cout<<"timepiere :"<<time_pierce<<" can hold the car of waiting !"<<endl;
        /********处理未上路的车辆**********/
        //遍历一遍所有车，恰好到达出发时刻，更新车辆  判断 道路是否堵死  堵死：不上路
        //超过出发时刻，判断是否堵死 堵死：不上路

        if(!car_multimap.empty())
        {
            map<int ,multimap<int, Car> >::iterator iter=car_multimap.begin();

            if(iter->second.empty())
            {
                iter=car_multimap.erase(iter);
            }

        while(iter!=car_multimap.end())
        {
            if(car_map_ready_number==car_num_restriction)
            {
                break;
            }
            multimap<int, Car>::iterator  pt_multimap=iter->second.begin();
            while(car_map_ready_number!=car_num_restriction)
            {

                if(pt_multimap==iter->second.end()&&car_map_ready_number!=car_num_restriction)
                {
                    ++iter;
                    break;
                }
                //cout<<pt_multimap->second.car_ID<<endl;
                car_map_ready.insert(make_pair(pt_multimap->second.car_ID,pt_multimap->second));
                ++car_map_ready_number;
                pt_multimap=iter->second.erase(pt_multimap);
            }
        }
        }
        cout<<"car_map_ready_number :"<<car_map_ready_number<<endl;
        if(!car_map_ready.empty())
        {
            map<int, Car>::iterator pt_car_map_ready=car_map_ready.begin();
            while(car_map_number!=car_num_restriction)
            {

                if(pt_car_map_ready==car_map_ready.end())
                {
                    break;
                }
                int car_id=pt_car_map_ready->first;
                if(car_map_ready[car_id].start_time<=time_pierce)//可以出发
                {

                    car_map_ready[car_id].cross_now=car_map_ready[car_id].start_cross;

                    renew_road_cross(car_id,car_map_ready,cross_map,road_map);/**确定next_cross,next_road**/

                    int next_Road=car_map_ready[car_id].next_road[0];

                    int i ;
                    int j=road_map[next_Road].road_length-1;
                    for(i=0;i!=road_map[next_Road].channel_num;++i)
                    {
                        if(road_map[next_Road].road_gesture[car_map_ready[car_id].start_cross][i][j]==0)
                        {
                            break;
                        }
                    }
                    if(i==road_map[next_Road].channel_num)//不能过
                    {
                        ++pt_car_map_ready;
                        continue;//开始下一辆车，这辆车仍为未出发。
                    }
                    if(i!=road_map[next_Road].channel_num)//能过，  需要更新很多信息
                    {
                        int M ;
                        int V=car_map_ready[car_id].max_speed;
                        if(V >road_map[next_Road].max_speed)
                        {
                            V =road_map[next_Road].max_speed;
                        }
                        for(M=j;M!=j-V;--M)
                        {
                            if(road_map[next_Road].road_gesture[car_map_ready[car_id].start_cross][i][M]!=0)/**有阻挡**/
                            {
                                break;
                            }
                        }

                        --car_map_ready_number;
                        ++car_map_number;
                        ++timepiere_car_num;
                        road_map[next_Road].road_gesture[car_map_ready[car_id].start_cross][i][M+1]=car_id;//更新道路状态

                        car_map[car_id]=car_map_ready[car_id];

                        car_map[car_id].road_locate.first=i;
                        car_map[car_id].road_locate.second=M+1;
                        car_map[car_id].is_start=1;
                        The_answer_map[car_id].push_back(car_id);
                        The_answer_map[car_id].push_back(time_pierce);//实际出发时间压入链表
                        car_map[car_id].road_now=next_Road;
                        car_map[car_id].cross_now=car_map_ready[car_id].next_cross[0];
                        The_answer_map[car_id].push_back(next_Road); //更新所经道路

                        renew_speed_now(car_id,car_map,road_map);

                        renew_road_cross(car_id,car_map,cross_map,road_map);

                        renew_direction(car_id,car_map,cross_map);

                        pt_car_map_ready=car_map_ready.erase(pt_car_map_ready);

                        continue;
                    }
                }
                else
                {
                    ++pt_car_map_ready;
                    continue;
                }

            }
        }
        cout<<"car_map_number :"<<car_map_number<<endl;
            cout<<"timepiere :"<<time_pierce<<" is OK !"<<endl;
            ++time_pierce;
    }
    }

        /***输出链表***/
        output(argv[4], The_answer_map);
        cout<<" The answer is Ok !"<<endl;
        system("pause");
        return 0;
}
