#include <iostream>
using namespace std;
struct partition {
	int pid;      // 进程号 
	int begin;    // 开始地址 
	int size;     // 进程大小 
	int status;   // 状态 0代表占用，1代表空闲 
	partition* next;  // 链表指针
	void setV(int pid, int begin, int size, int status, partition* next=NULL){
		this->pid = pid;
		this->begin = begin;
		this->size = size;
		this->status = status;
		this->next = next;
	}
};
partition* head = new partition;

void initNode(struct partition* p) {
	if (p == NULL) {	//如果为空则新创建一个 
		p = new partition;
	}
	p->setV(-1, 0, 640, 1);
}

void printNode(partition* node = head) {
	cout << "                        内存情况                        \n"
		<< " -------------------------------------------------------\n"
		<< "| 起始地址\t结束地址\t大小\t状态\t任务id\t|\n";
	while (node != NULL) {
		if (node->status == 1) {
			cout << "| " << node->begin + 1 << "\t\t" << node->begin + node->size << "\t\t" << node->size << "KB\tfree\t 无\t|\n";
		}
		else {
			cout << "| " << node->begin + 1 << "\t\t" << node->begin + node->size << "\t\t" << node->size << "KB\tbusy\t " << node->pid << "\t|\n";
		}
		node = node->next;
	}
	cout << " -------------------------------------------------------\n";
}
void Free(int pid, int size=0, struct partition* node=head) {
	if (node->next == NULL && node->pid == -1) {
		printf("还没有分配任何任务！\n");
	}
	cout << "作业" << pid << " 释放" << size << " KB\n";
	while (node != NULL) {
		if (node->next != NULL and node->next->pid == pid) { // 必须通过pre节点处理的情况
			if (node->status == 1 && node->next->status == 0) {
				/*释放空间的上一块空间空闲
				  需要将两个空间拼接起来*/
				node->size += node->next->size;
				partition* q = node->next;
				node->next = node->next->next;
				delete q;
				if (node->next->status == 1) { //下一个空间是空闲空间时 
					node->size = node->size + node->next->size;
					partition* q = node->next;
					node->next = node->next->next;
					delete q;
				}
				break;
			}
		}
		else {  //只需要修改本节点的情况
			if (node->pid == pid) {
				node->status = 1;
				node->pid = -1;
				if (node->next != NULL && node->next->status == 1) { //下一个空间是空闲空间时 
					node->size += node->next->size;
					partition* del = node->next;
					node->next = node->next->next;
					delete del;
				}
				break;
			}

			if (node->next == NULL) {  //任务id不匹配时 
				printf("没有此任务！\n");
				break;
			}
		}
		node = node->next;
	}
	printNode();
}

void FirstFit(int pid, int size) {
	cout << "作业" << pid << " 申请" << size << " KB\n";
	partition* node = head;
	while (node != NULL) {
		if (node->status == 1 and node->size >= size) {  //有可用的空间 
			if (node->size > size) {  //剩余空间充足的情况
				//注意，我们令一个node指向内存的位置不变
				//分配后剩余的空间 
				partition* rest = new partition;
				rest->begin = node->begin + size;
				rest->size = node->size - size;
				rest->status = 1;
				rest->pid = -1;
				rest->next = node->next;
				//分配的空间 
				node->pid = pid;
				node->size = size;
				node->status = 0;
				node->next = rest;
			}
			else if (node->size == size) { //需求空间和空闲空间大小相等时 
				node->pid = pid;
				node->status = 0;
			}
			break;
		}
		else {  //node不可用
			if (node->next == NULL) {  //内存用尽了
				cout << "分配失败，内存空间不足！\n";
				break;
			}
			else {  //内存没有用尽
				node = node->next;
			}
		}	
	}
	printNode();
}

void BestFit(int pid, int size, partition* node=head) {
	cout << "作业" << pid << " 申请" << size << " KB\n";
	//最佳块指针 
	partition* result = NULL;
	//遍历寻找最佳的空闲块 
	while (node != NULL) {
		if (node->status == 1 && node->size >= size) {  //空闲的空间 
			if (result == NULL) {
				result = node;
			}
			else if (result->size > node->size) {
				result = node;
			}
		}
		node = node->next;
	}

	if (result->size > size) {  			//最佳空闲块的大小大于需求大小
		//注意，我们令一个node指向内存的位置不变
		//分配后剩余的空间 
		partition* rest = new partition;
		rest->begin = result->begin + size;
		rest->size = result->size - size;
		rest->status = 1;
		rest->pid = -1;
		rest->next = result->next;
		//分配的空间 
		result->pid = pid;
		result->size = size;
		result->status = 0;
		result->next = rest;
	}
	else if (result->size == size) {  	//最佳空闲块空间大小和需求相等
		result->pid = pid;
		result->size = size;
		result->status = 0;
	}
	printNode();
}

void destory( partition* node=head) {
	partition* del = node;
	while (node != NULL) {
		node = node->next;
		delete del;
		del = node;
	}
}

int main() {
	// node为整个空间 
	initNode(head);			//初始化主链 
	int option;
	int operate[2][11] = {
		{1,1,1,0,1,0,0,1,1,1,0},  // 1申请，0释放
		{1,2,3,2,4,3,1,5,6,7,6},  // pid
	};
	int p_size[8] = { -1,130,60,100,200,140,60,50 }; //进程号从1-7
	cout << " -------------------动态分区算法演示--------------------\n\n";
	//if (option == 1) {
	//	FirstFit(1, 130);	//作业1 申请130 KB
	//	FirstFit(2, 60);	//作业2 申请60 KB
	//	FirstFit(3, 100);	//作业3 申请100 KB
	//	Free(2, 60);		//作业2 释放60 KB	
	//	FirstFit(4, 200);	//作业4 申请200 KB		
	//	Free(3, 100);		//作业3 释放100 KB		
	//	Free(1, 130);		//作业1 释放130 KB		
	//	FirstFit(5, 140);	//作业5 申请140 KB				
	//	FirstFit(6, 60);	//作业6 申请60 KB		
	//	FirstFit(7, 50);	//作业7 申请50 KB		
	//	Free(6, 60);		//作业6 释放60 KB
	//}
	//else {
	//	BestFit(1, 130);	//作业1 申请130 KB
	//	BestFit(2, 60);	//作业2 申请60 KB
	//	BestFit(3, 100);	//作业3 申请100 KB
	//	Free(2, 60);		//作业2 释放60 KB	
	//	BestFit(4, 200);	//作业4 申请200 KB		
	//	Free(3, 100);		//作业3 释放100 KB		
	//	Free(1, 130);		//作业1 释放130 KB		
	//	BestFit(5, 140);	//作业5 申请140 KB				
	//	BestFit(6, 60);	//作业6 申请60 KB		
	//	BestFit(7, 50);	//作业7 申请50 KB		
	//	Free(6, 60);			//作业6 释放60 KB
	//}

	cout << "|-------------------------------------------------------|\n"
		<< "|                      首先适配算法                     |\n"
		<< "|-------------------------------------------------------|\n";
	printf("按回车继续...");
	getchar();
	option = 1;
	for (int i = 0; i < 11; i++) {  //11个操作
			
		if (operate[0][i] == 1) { //申请
			if (option == 1)
				FirstFit(operate[1][i], p_size[operate[1][i]]);
			else
				BestFit(operate[1][i], p_size[operate[1][i]]);
		}
		else {
			Free(operate[1][i], p_size[operate[1][i]]);
		}
			
	}
	destory(head);	//销毁链表

	cout << "\n\n|-------------------------------------------------------|\n"
		<< "|                      最佳适配算法                     |\n"
		<< "|-------------------------------------------------------|\n";
	printf("按回车继续...");
	getchar();
	option = 2;
	head = new partition;
	initNode(head);	//重新初始化 
	for (int i = 0; i < 11; i++) {  //11个操作

		if (operate[0][i] == 1) { //申请
			if (option == 1)
				FirstFit(operate[1][i], p_size[operate[1][i]]);
			else
				BestFit(operate[1][i], p_size[operate[1][i]]);
		}
		else {
			Free(operate[1][i], p_size[operate[1][i]]);
		}

	}
	destory(head);	//销毁链表
	system("pause");

}