/*
 * robowork.cxx
 * 
 * Copyright 2017 gzj <function@gzj>
 * 
 * V1.3
 * rebuild for file I/O and permutation
 * 
 */
 
 
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
using namespace std;

/**********************************/
/************ CONSTANT ************/
/**********************************/

const int moving_time=8;        //the time the robot moving between two box
const int checking_time = 3;    //the time the robot check one raid
const int catching_time = 6;    //the time the robot catch one box
const int putdown_time = 6;		//the time the robot putdown one box

const string ans_1 = "0123456";  //four possible answers
const string ans_2 = "1234560";
const string ans_3 = "6543210";
const string ans_4 = "0654321";

struct qnode{
	string pos_box;     //where the boxes are now range from 0 to 6
	int pos_number;     //the number of known box range from 0 to 7
	int pos_robot;      //where the robot are now range from -1 to 6
	long used_time;     //how long it has used
	string history;     //how the robot come to the state
						//1:move forword	2:move backword 
						//3:check  			4:catch and store
						//5:put down        0:nothing
	char store;   		//-1 for nothing
//	qnode *next;
};

/**********************************/
/************ VARIABLE ************/
/**********************************/

int start_box[7]={0,1,2,3,4,5,6};
string start_pos="6253140"; //0 is for invalid
qnode queue[1000000];  //queue of node
long head=0,tail=-1;	//head,tail for queue
bool visited[10000000] = {false}; //if visited
int co,flag[4]={1,1,1,1};
ofstream file;

/**********************************/
/************ FUNCTION ************/
/**********************************/

int my_cmp(qnode x, qnode y){
	return x.used_time < y.used_time;
}

long get_hex(qnode q){
	long hex=0;
	for (int i=0;i<q.pos_number;i++){
		hex = hex*7 + q.pos_box[i]-'0';
	}
	hex = hex*7 + q.pos_robot;
	return hex;
}

int get_new_state(qnode now){
	qnode q;
	int pos_robot=now.pos_robot;
	long hex;
	
	//1:move forword
	q = now;
	if (q.pos_robot<7){
		q.pos_robot++;
		
		hex=get_hex(q);
		if (!visited[hex]){
			visited[hex]=true;
			q.used_time+=moving_time;
			q.history+="1";
			queue[head++]=q;
		}
	}
	//2:move backword
	q = now;
	if (q.pos_robot>0){
		q.pos_robot--;
		
		hex=get_hex(q);
		if (!visited[hex]){
			visited[hex]=true;
			q.used_time+=moving_time;
			q.history+="2";
			queue[head++]=q;
		}
	}

	//4:catch and store
	q = now;
	if ((pos_robot>=0) && (q.store=='0') && (q.pos_box[pos_robot]>'0')) {
		q.store = q.pos_box[pos_robot];
		q.pos_box[pos_robot]='0';
		
		hex=get_hex(q);
		if (!visited[hex]){
			visited[hex]=true;
			q.used_time+=putdown_time;
			q.history+="4";
			queue[head++]=q;
		}
	}
	//5:put down
	q = now;
	if ((pos_robot>=0) && (q.store>'0')) {
		q.pos_box[pos_robot]=q.store;
		q.store = '0';
		
		hex=get_hex(q);
		if (!visited[hex]){
			visited[hex]=true;
			q.used_time+=putdown_time;
			q.history+="5";
			queue[head++]=q;
		}
	}
	return 0;
}

int output(qnode q){	
/*	file<<" starting pos: " 	<< start_pos  << endl;
	file<<" pos of box: " 			<< q.pos_box   		 << endl;
	file<<" history of operating: " 	<< q.history   		 << endl;
	file<<" counting of operating: " << q.history.length()<< endl;
	file<<" used time: " 			<< q.used_time 		 << endl; */
	file << start_pos <<' ';
	file << q.pos_box <<' ';
	file << q.history << endl<< endl;
	return 0;
}

int check_answer(qnode now){
	string str="";
	str=now.pos_box;
	
	if (flag[0] && (str==ans_1)){
		output(now);
		flag[0]=0;
		cout<<endl;
		return 1;
	}
	if (flag[1] && (str==ans_2)){
		output(now);
		flag[1]=0;
		cout<<endl;
		return 1;
	}
	if (flag[2] && (str==ans_3)){
		output(now);
		flag[2]=0;
		cout<<endl;
		return 1;
	}
	if (flag[3] && (str==ans_4)){
		output(now);
		flag[3]=0;
		cout<<endl;
		return 1;
	}
	return 0;
}

int get_next_pos(void){
	string str="";
	char c;
	for (int i=0;i<7;i++){
		c='0'+start_box[i];
		str+=c;
	}
	start_pos=str;
	next_permutation(start_box,start_box+7);
	return 0;
}

int operate(void){
	while(1){
		qnode now=queue[++tail];				//dequeue
		get_new_state(now);						//enqueue
		if (check_answer(now)){		//ans??
			return 0;
		}
	}
}

/**********************************/
/************** MAIN **************/
/**********************************/
int main(){
	
	//initial
	file.open("robowork1.3.txt");
	file<<" 1:move forword       2:move backword"<<endl;
	file<<" 4:catch and store    5:put down"<<endl<<endl;
	
	
	for(int i=0;i<10000;i++){
		get_next_pos();
		head=0;tail=-1;
		qnode q={start_pos,7,-1,0,"",'0'};
		queue[head] = q;
		head++;
		operate();
	}
	
	file.close();
	return 0;
}


/*
* 
* 
* 
* 	//3:check  	
	q = now;
	if ((pos_robot>=0) && (q.pos_box[pos_robot]==-1)){
		//q.pos_box[pos_robot]=check();
		q.pos_number++;
		q.used_time+=checking_time;
		q.history+="3";
		queue[head++]=q;
	}
* sort(queue+tail+1,queue+head,my_cmp);	//sort
* 
* 
*/
