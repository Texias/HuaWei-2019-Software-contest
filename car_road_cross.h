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
const int rand_size=4; /****һ��·�ڵĵ�·����****/
void exchange_char(char &a, char &b);/******���������ַ���ֵ*****/
void exchange(int &a,int &b);/**********������������ֵ************/


class Car               /**********��������**********/
{
public:
    int car_ID; /**����ID*/
    int start_cross;/**����·��***/
    int end_cross;/**Ŀ�ĵ�·��***/
    int max_speed;/**����ٶ�**/
    int start_time;/**�ƻ�����ʱ��**/
    //Car_gesture gesture;
    int is_start;  /**�˶�״̬��δ������-1 �ѳ������ȴ���0 ��ֹ��1�� �����յ㣺2***/
    //int fact_start_time;/**ʵ�ʳ���ʱ��**/
    int speed_now;/**��һ�ε��ȿ���ʻ�ľ���**/
    int road_now;/**�������ڵ�·**/
    //int cross_last;/*****��һ��·��*******/
    int cross_now;/**�����ڵ�·�ϼ���ȥ����·��**/
    pair<int,int> road_locate;/**�������ڵ�·λ��:���ڳ����ͳ�������λ��**/
    int next_road[rand_size];/**�ƻ�ʻ�����һ���������ȼ��ĵ�·:��0����ʾû�У����ܰ���������·***/
    int next_cross[rand_size];/**����cross_now�ļƻ�ʻ�����һ��·�ڵ����ȼ����У����ܰ���cross_last) ��0����ʾû��***/
    char direction[rand_size];/***********��һ��·�ڵķ���,L:��ת  R����ת  D��ֱ�� N����ʾ��ͷ**************/
    //int is_pass_cross;/**��һ�ε����Ƿ񾭹�·��***/
    //list<int> time_road; /********��¼������ID,����ʱ�䣬·����Ϣ********/

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

class Cross      /*******************·�������Լ�·��״̬*******************/
{
public:
    int cross_ID;/**·��ID**/
    int road[rand_size];      /*************��·����Χ�ĵ�·������Ϊ-1��***************/
    int in_rand[rand_size]; /**�����·�ڵĵ�·���������У�������-1��-1��ʾû�е�·��**/
    int other_cross[rand_size];/**�������·����ͨ��·�ڲ����ڣ�0   ��Ӧ��·�����ӵ�·��˳ʱ��˳��õ�**/
    /*int in_cross[rand_size];   �����·�ڵ�����*/
    Cross():cross_ID(0)
    {
        for(int i=0;i!=rand_size;++i)
        {
            in_rand[i]=-1;
            other_cross[i]=0;
        }
    }
};
class Road      /*************��·����******************/
{
public:
    int road_ID;/***��·ID**/
    int road_length;/***��·����**/
    int max_speed;/**��·����***/
    int channel_num;/***������Ŀ***/
    int start_cross;/***��ʼ·��***/
    int end_cross;/***��ֹ·��***/
    int isduplex;/****�Ƿ�˫�� ˫��1 ����0**/
    map<int ,int **> road_gesture;/********��ʾfirstΪ·�����ĸ�����·�Ŀ�λ״̬��Ҳ�����������Ԫ��***/
    //int **road_gesture;/***��·״̬���̻�ÿһ�����ڸõ�·�еĵ�ǰ λ��**/
    Road():road_ID(0),road_length(0),max_speed(0),channel_num(0),start_cross(0),end_cross(0),isduplex(0){}
};
/*******************����ÿ��·�ڵ���ڵ�·�������ͨ·��******************/
void set_cross(map<int,Cross>& cross_map,map<int,Road>& road_map);

/**********************************************����car��·����Ϣ������map******************************/
int make_car_multimap(const char *direction,map<int ,multimap<int ,Car > >&car_multimap);

/*********************�����������·��****************************/
void output(const char* direction,map<int ,list<int> >&car_map);

/************************************����cross����Ϣ������map*************************************************/
void make_cross_map(const char *direction,map<int ,Cross >&cross_map);

/************************************����road����Ϣ������map*************************************************/
int make_road_map(const char *direction,map<int ,Road >&road_map);

/***************************����·��״̬��Ϣ����ʼ·�ڡ���ֹ·�ڣ�������ʼ����ֹ֮�����ֵ̽���Ŀ**********************************/
int test_distance(int cross_begin,int cross_end,map<int ,Cross>&cross_map);

/*******************************��cross_now���յ�crossȷ��next_cross[]��next_road[]*******************************************/
void renew_road_cross(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map,map<int,Road>& road_map);

void renew_direction(int car_id,map<int ,Car>&car_map,map<int ,Cross>&cross_map);/******���¸ó��ڸ�·�ڵ�ת�����********/

/*******���³���********/
void renew_speed_now(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map);
pair<int, int> next_location(int car_id,map<int ,Car>&car_map,map<int ,Road >&road_map);/***first: -2:ȷ����ֹ��ͣ�ڵ�ǰ��· -1������ȷ����ֹ����Ϊ�ȴ� ��������ȷ����һλ��**/

void copy_car_map(map<int ,Car>&real_one , map<int ,Car>&copy_one);  /**���Ƶ�ͼ**/
void copy_road_map(map<int ,Road>&real_one , map<int ,Road>&copy_one);
int test_lock_num(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map);
//void copy_cross_map(map<int ,Cross>&real_one , map<int ,Cross>&copy_one);
pair<int ,int > test_lock(map<int ,Car>&car_map , map<int ,Road>&road_map , map<int ,Cross>&cross_map);/**�����Ƿ�����**/

#endif // CAR_ROAD_CROSS_H_INCLUDED
