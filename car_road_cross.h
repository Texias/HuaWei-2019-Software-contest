#ifndef CAR_ROAD_CROSS_H_INCLUDED
#define CAR_ROAD_CROSS_H_INCLUDED
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <iterator>
#include <utility>
#include <list>
#include <set>
#include <time.h>
#include <stdlib.h>
using namespace std;
const int multimap_num=2000;
const int rand_size=4; /****一个路口的道路数量****/
void exchange_char(char &a, char &b);/******交换两个字符的值*****/
void exchange(int &a,int &b);/**********交换两个数的值************/


class Car               /**********车辆属性**********/
{
public:
    int car_ID; /**车辆ID*/
    int start_cross;/**出发路口***/
    int end_cross;/**目的地路口***/
    int max_speed;/**最高速度**/
    int start_time;/**计划出发时间**/
    //Car_gesture gesture;
    int is_start;  /**运动状态：未出发：-1 已出发（等待：0 终止：1） 到达终点：2***/
    //int fact_start_time;/**实际出发时间**/
    int speed_now;/**下一次调度可行驶的距离**/
    int road_now;/**现在所在道路**/
    //int cross_last;/*****上一个路口*******/
    int cross_now;/**现在在道路上即将去往的路口**/
    pair<int,int> road_locate;/**现在所在道路位置:所在车道和车道具体位置**/
    int next_road[rand_size];/**计划驶入的下一条具有优先级的道路:用0来表示没有，可能包括本条道路***/
    int next_cross[rand_size];/**基于cross_now的计划驶入的下一个路口的优先级排列（可能包含cross_last) 用0来表示没有***/
    char direction[rand_size];/***********下一个路口的方向,L:左转  R：右转  D：直行 N：表示掉头**************/
    //int is_pass_cross;/**这一次调度是否经过路口***/
    //list<int> time_road; /********记录车辆的ID,出发时间，路径信息********/

    Car():car_ID(0),start_cross(0),end_cross(0),max_speed(0),start_time(0),is_start(-1),speed_now(0),road_now(0),cross_now(0),road_locate(0,0)
    {
        for(int i=0;i!=rand_size;++i)
        {
            next_road[i]=0;
            next_cross[i]=0;
            direction[i]='N';
        }

    }
};

class Cross      /*******************路口属性以及路口状态*******************/
{
public:
    int cross_ID;/**路口ID**/
    int road[rand_size];      /*************该路口周围的道路（无则为-1）***************/
    int in_rand[rand_size]; /**进入该路口的道路的升序排列，包含了-1（-1表示没有道路）**/
    int other_cross[rand_size];/**其它与该路口连通的路口不存在：0   对应于路口所接道路的顺时针顺序得到**/
    /*int in_cross[rand_size];   进入该路口的其它*/
    Cross():cross_ID(0)
    {
        for(int i=0;i!=rand_size;++i)
        {
            in_rand[i]=-1;
            other_cross[i]=0;
        }
    }
};
class Road      /*************道路属性******************/
{
public:
    int road_ID;/***道路ID**/
    int road_length;/***道路长度**/
    int max_speed;/**道路限速***/
    int channel_num;/***车道数目***/
    int start_cross;/***开始路口***/
    int end_cross;/***终止路口***/
    int isduplex;/****是否双向 双向：1 单向0**/
    map<int ,int **> road_gesture;/********表示first为路口起点的该条道路的坑位状态，也就是最多两个元素***/
    //int **road_gesture;/***道路状态，刻画每一辆车在该道路中的当前 位置**/
    Road():road_ID(0),road_length(0),max_speed(0),channel_num(0),start_cross(0),end_cross(0),isduplex(0){}
};
/*******************设置每个路口的入口道路升序和邻通路口******************/
void set_cross(map<int,Cross>& cross_map,map<int,Road>& road_map);

/**********************************************输入car的路径信息并构建map******************************/
int make_car_multimap(const char *direction,map<int ,multimap<int ,Car > >&car_multimap);

/*********************输出各辆车的路径****************************/
void output(const char* direction,map<int ,list<int> >&car_map);

/************************************输入cross的信息并构建map*************************************************/
void make_cross_map(const char *direction,map<int ,Cross >&cross_map);

/************************************输入road的信息并构建map*************************************************/
int make_road_map(const char *direction,map<int ,Road >&road_map);

/***************************输入路口状态信息、起始路口、终止路口，返回起始到终止之间的最短街道数目**********************************/
int test_distance(int cross_begin,int cross_end,map<int ,Cross>&cross_map);

/*******************************由cross_now和终点cross确定next_cross[]和next_road[]*******************************************/
void renew_road_cross(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map,map<int,Road>& road_map);

void renew_direction(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map);/******更新该车在该路口的转弯情况********/

/*******更新车速********/
void renew_speed_now(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map);
pair<int, int> next_location(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map);/***first: -2:确定终止，停在当前道路 -1：不能确定终止，还为等待 其它：能确定下一位置**/

void copy_car_map(map<int ,Car>&real_one , map<int ,Car>&copy_one);  /**复制地图**/
void copy_road_map(map<int ,Road>&real_one , map<int ,Road>&copy_one);
int test_lock_num(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map);
//void copy_cross_map(map<int ,Cross>&real_one , map<int ,Cross>&copy_one);
pair<int ,int > test_lock(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map);/**测试是否死锁**/

#endif // CAR_ROAD_CROSS_H_INCLUDED
