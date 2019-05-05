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

    int Total_car_num_real=make_car_multimap(argv[1],car_multimap);/***�����ܵ�����,�����յ�һ����һ������Ϊ��ʱ��ֹ**/

    int Total_road_location=make_road_map(argv[2],road_map);       /**��·�г����ܵĿ�λ*/


    int car_num_restriction =static_cast<int>(Total_road_location/4); /**����ͼ�г�����Ŀ�����ƣ����������ؼ��ĵط�������������������*/

    make_cross_map(argv[3],cross_map);
    set_cross(cross_map,road_map);       /******ȷ��·��״̬��Ϣ*******/
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
    int time_pierce=1;/**ʱ��Ƭ**/
    int goto_next_round=0;
    int timepiere_car_num=0;

    car_num_restriction=1000;
    while(time_pierce)/***��ʼ����***/
    {
        cout<<"the total car_num is :"<<Total_car_num<<endl;

        cout<<"the car_num_restrition is :"<<car_num_restriction<<endl;

        cout<<"the car on the road is : "<<timepiere_car_num<<endl;
        if(Total_car_num==0)
        {
            time_to_break=1;
            break;
        }
       //��һ��������ɺ��·�г�������Ŀ������ȷ���Ƿ�Ӧ�� ����

        /**�����ڵ�·�еĳ���**/
        for(map<int ,Road>::iterator iter_road_map=road_map.begin();iter_road_map!=road_map.end();++iter_road_map)/**����ÿ����·***/
        {
            for(int i=0;i!=iter_road_map->second.road_length;++i)/***����һ����·�����г�������ȷ��״̬**/
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
                            if(i-m<=car_map[car_id].speed_now)/**ǰ���г������һ��赲**/
                            {
                                int pre_car_id=iter_road_map->second.road_gesture[pt_cross->first][j][m]; /**�赲����ID**/
                                if(car_map[pre_car_id].is_start==0)/**ǰ��Ϊ�ȴ�����ó�ҲΪ�ȴ�**/
                                {
                                    car_map[car_id].is_start=0; /**�ó�ҲΪ�ȴ�**/
                                    break;
                                }
                                if(car_map[pre_car_id].is_start==1)/**ǰ��Ϊ��ֹ**/
                                {
                                    car_map[car_id].is_start=1;/**�ó�ҲΪ��ֹ**/
                                    car_map[car_id].road_locate.second= m+1;/**���³���״̬**/
                                    //���µ�·״̬
                                    iter_road_map->second.road_gesture[pt_cross->first][j][i]=0;
                                    iter_road_map->second.road_gesture[pt_cross->first][j][m+1]=car_id;

                                    break;
                                }
                            }
                            if(i-m>car_map[car_id].speed_now)/**ǰ���г����������赲**/
                            {
                                car_map[car_id].is_start=1;/**�ó�ҲΪ��ֹ**/
                                car_map[car_id].road_locate.second=i-car_map[car_id].speed_now;/**���³���״̬**/
                                //���µ�·״̬
                                iter_road_map->second.road_gesture[pt_cross->first][j][i]=0;
                                iter_road_map->second.road_gesture[pt_cross->first][j][i-car_map[car_id].speed_now]=car_id;

                                break;
                            }
                        }
                        if(m==-1)/**ǰ���޳�**/
                        {
                            if(car_map[car_id].speed_now>i)/**Ҫ��·��**/
                            {
                                car_map[car_id].is_start=0;/**�ó�Ϊ�ȴ�**/
                                continue;
                            }
                            else/**����·��**/
                            {
                                //���µ�·��Ϣ�ͳ���״̬
                                car_map[car_id].is_start=1;/**�ó�Ϊ��ֹ**/
                                car_map[car_id].road_locate.second=i-car_map[car_id].speed_now;/**���³���״̬**/
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
        /**�����ڵȴ��ĳ���**/
        map<int, Car> copy_car;
        map<int, Road> copy_road;
        copy_car_map(car_map , copy_car);
        copy_road_map(road_map , copy_road);

        //copy_cross_map(cross_map , copy_cross);/////��ʵcross��̫��Ҫ
        while(1)  /////////ȷ����ε��Ȳ����������
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
                if(will_lock.first==10000)/**����ֵ���֣����ʱ�������е�ͼ �ı��·�������� ������һ�飬ȷ��һ���ܳ����**********/
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
        ////////////һ�������������
        while(1)
        {

            int no_change=0;/**��ֹһ����һ��**/
            int total_wait_num=0;
            for(map<int ,Cross>::iterator iter_cross_map=cross_map.begin();iter_cross_map!=cross_map.end();++iter_cross_map)/**������ȸ���·��**/
            {
                //int cross_id=iter_cross_map->first;
                //int goto_next_cross=0;/**����Ƿ�������һ·��**/
                while(1)/**���ѭ�����ȸ�·�ڵ�·ֱ����·�����г�����Ϊ��ֹ״̬����ѭ��һ��û�г�����Ϊ��ֹ״̬**/
                {

                    int PM2 =0;
                    int wait_num=0;/**һ��·�ڳ��Ӵ��ڵȴ�״̬������**/
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
                                        if(car_map[car_id].is_start==0) /***�Ҹó����ڵȴ�״̬**/
                                        {

                                            ++wait_num;
                                            int stock;////�жϸó�ǰ���Ƿ�����赲
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
                                                    --Total_car_num;
                                                    --timepiere_car_num;
                                                    --wait_num;
                                                    car_map.erase(car_id);
                                                    --car_map_number;
                                                    continue;   /**������һ���� **/
                                                }
                                                //cout<<"car_id : "<<car_id<<" direction[0] : "<<car_map[car_id].gesture.direction[0]<<endl;
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
                                                        The_answer_map[car_id].push_back(car_map[car_id].road_now); //����������·
                                                        renew_speed_now(car_id,car_map,road_map);
                                                        renew_road_cross(car_id,car_map,cross_map,road_map);
                                                        renew_direction(car_id,car_map,cross_map);
                                                        road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                        road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                        int PV=0;
                                                        while(car_map[car_id].direction[PV]=='N')/****��һ���ȼ�ΪN�򰴵ڶ����ȼ��ܣ�֪�������ȼ���ΪN***/
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
                                                            The_answer_map[car_id].push_back(car_map[car_id].road_now); //����������·
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            int PV=0;
                                                            while(car_map[car_id].direction[PV]=='N')/****��һ���ȼ�ΪN�򰴵ڶ����ȼ��ܣ�֪�������ȼ���ΪN***/
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
                                                            The_answer_map[car_id].push_back(car_map[car_id].road_now); //����������·
                                                            renew_speed_now(car_id,car_map,road_map);
                                                            renew_road_cross(car_id,car_map,cross_map,road_map);
                                                            renew_direction(car_id,car_map,cross_map);
                                                            road_map[road_id].road_gesture[the_cross_now][n][m]= 0;//���µ�·״̬
                                                            road_map[car_map[car_id].road_now].road_gesture[the_cross_other][P.first][P.second]=car_id;
                                                            int PV=0;
                                                            while(car_map[car_id].direction[PV]=='N')/****��һ���ȼ�ΪN�򰴵ڶ����ȼ��ܣ�ֱ�������ȼ���ΪN***/
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
                                                --no_change;
                                                --PM2;
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
                    if(PM2==0)/***��·��û�д��ڵȴ�״̬�ĳ����������е�һ���ȼ����������ڵȴ�״̬**/
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
        /********����δ��·�ĳ���**********/
        //����һ�����г���ǡ�õ������ʱ�̣����³���  �ж� ��·�Ƿ����  ����������·
        //��������ʱ�̣��ж��Ƿ���� ����������·

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
                if(car_map_ready[car_id].start_time<=time_pierce)//���Գ���
                {

                    car_map_ready[car_id].cross_now=car_map_ready[car_id].start_cross;

                    renew_road_cross(car_id,car_map_ready,cross_map,road_map);/**ȷ��next_cross,next_road**/

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
                    if(i==road_map[next_Road].channel_num)//���ܹ�
                    {
                        ++pt_car_map_ready;
                        continue;//��ʼ��һ��������������Ϊδ������
                    }
                    if(i!=road_map[next_Road].channel_num)//�ܹ���  ��Ҫ���ºܶ���Ϣ
                    {
                        int M ;
                        int V=car_map_ready[car_id].max_speed;
                        if(V >road_map[next_Road].max_speed)
                        {
                            V =road_map[next_Road].max_speed;
                        }
                        for(M=j;M!=j-V;--M)
                        {
                            if(road_map[next_Road].road_gesture[car_map_ready[car_id].start_cross][i][M]!=0)/**���赲**/
                            {
                                break;
                            }
                        }

                        --car_map_ready_number;
                        ++car_map_number;
                        ++timepiere_car_num;
                        road_map[next_Road].road_gesture[car_map_ready[car_id].start_cross][i][M+1]=car_id;//���µ�·״̬

                        car_map[car_id]=car_map_ready[car_id];

                        car_map[car_id].road_locate.first=i;
                        car_map[car_id].road_locate.second=M+1;
                        car_map[car_id].is_start=1;
                        The_answer_map[car_id].push_back(car_id);
                        The_answer_map[car_id].push_back(time_pierce);//ʵ�ʳ���ʱ��ѹ������
                        car_map[car_id].road_now=next_Road;
                        car_map[car_id].cross_now=car_map_ready[car_id].next_cross[0];
                        The_answer_map[car_id].push_back(next_Road); //����������·

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

        /***�������***/
        output(argv[4], The_answer_map);
        cout<<" The answer is Ok !"<<endl;
        system("pause");
        return 0;
}
