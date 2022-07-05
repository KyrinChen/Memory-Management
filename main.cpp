#include <iostream>
using namespace std;
struct partition {
	int pid;      // ���̺� 
	int begin;    // ��ʼ��ַ 
	int size;     // ���̴�С 
	int status;   // ״̬ 0����ռ�ã�1������� 
	partition* next;  // ����ָ��
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
	if (p == NULL) {	//���Ϊ�����´���һ�� 
		p = new partition;
	}
	p->setV(-1, 0, 640, 1);
}

void printNode(partition* node = head) {
	cout << "                        �ڴ����                        \n"
		<< " -------------------------------------------------------\n"
		<< "| ��ʼ��ַ\t������ַ\t��С\t״̬\t����id\t|\n";
	while (node != NULL) {
		if (node->status == 1) {
			cout << "| " << node->begin + 1 << "\t\t" << node->begin + node->size << "\t\t" << node->size << "KB\tfree\t ��\t|\n";
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
		printf("��û�з����κ�����\n");
	}
	cout << "��ҵ" << pid << " �ͷ�" << size << " KB\n";
	while (node != NULL) {
		if (node->next != NULL and node->next->pid == pid) { // ����ͨ��pre�ڵ㴦������
			if (node->status == 1 && node->next->status == 0) {
				/*�ͷſռ����һ��ռ����
				  ��Ҫ�������ռ�ƴ������*/
				node->size += node->next->size;
				partition* q = node->next;
				node->next = node->next->next;
				delete q;
				if (node->next->status == 1) { //��һ���ռ��ǿ��пռ�ʱ 
					node->size = node->size + node->next->size;
					partition* q = node->next;
					node->next = node->next->next;
					delete q;
				}
				break;
			}
		}
		else {  //ֻ��Ҫ�޸ı��ڵ�����
			if (node->pid == pid) {
				node->status = 1;
				node->pid = -1;
				if (node->next != NULL && node->next->status == 1) { //��һ���ռ��ǿ��пռ�ʱ 
					node->size += node->next->size;
					partition* del = node->next;
					node->next = node->next->next;
					delete del;
				}
				break;
			}

			if (node->next == NULL) {  //����id��ƥ��ʱ 
				printf("û�д�����\n");
				break;
			}
		}
		node = node->next;
	}
	printNode();
}

void FirstFit(int pid, int size) {
	cout << "��ҵ" << pid << " ����" << size << " KB\n";
	partition* node = head;
	while (node != NULL) {
		if (node->status == 1 and node->size >= size) {  //�п��õĿռ� 
			if (node->size > size) {  //ʣ��ռ��������
				//ע�⣬������һ��nodeָ���ڴ��λ�ò���
				//�����ʣ��Ŀռ� 
				partition* rest = new partition;
				rest->begin = node->begin + size;
				rest->size = node->size - size;
				rest->status = 1;
				rest->pid = -1;
				rest->next = node->next;
				//����Ŀռ� 
				node->pid = pid;
				node->size = size;
				node->status = 0;
				node->next = rest;
			}
			else if (node->size == size) { //����ռ�Ϳ��пռ��С���ʱ 
				node->pid = pid;
				node->status = 0;
			}
			break;
		}
		else {  //node������
			if (node->next == NULL) {  //�ڴ��þ���
				cout << "����ʧ�ܣ��ڴ�ռ䲻�㣡\n";
				break;
			}
			else {  //�ڴ�û���þ�
				node = node->next;
			}
		}	
	}
	printNode();
}

void BestFit(int pid, int size, partition* node=head) {
	cout << "��ҵ" << pid << " ����" << size << " KB\n";
	//��ѿ�ָ�� 
	partition* result = NULL;
	//����Ѱ����ѵĿ��п� 
	while (node != NULL) {
		if (node->status == 1 && node->size >= size) {  //���еĿռ� 
			if (result == NULL) {
				result = node;
			}
			else if (result->size > node->size) {
				result = node;
			}
		}
		node = node->next;
	}

	if (result->size > size) {  			//��ѿ��п�Ĵ�С���������С
		//ע�⣬������һ��nodeָ���ڴ��λ�ò���
		//�����ʣ��Ŀռ� 
		partition* rest = new partition;
		rest->begin = result->begin + size;
		rest->size = result->size - size;
		rest->status = 1;
		rest->pid = -1;
		rest->next = result->next;
		//����Ŀռ� 
		result->pid = pid;
		result->size = size;
		result->status = 0;
		result->next = rest;
	}
	else if (result->size == size) {  	//��ѿ��п�ռ��С���������
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
	// nodeΪ�����ռ� 
	initNode(head);			//��ʼ������ 
	int option;
	int operate[2][11] = {
		{1,1,1,0,1,0,0,1,1,1,0},  // 1���룬0�ͷ�
		{1,2,3,2,4,3,1,5,6,7,6},  // pid
	};
	int p_size[8] = { -1,130,60,100,200,140,60,50 }; //���̺Ŵ�1-7
	cout << " -------------------��̬�����㷨��ʾ--------------------\n\n";
	//if (option == 1) {
	//	FirstFit(1, 130);	//��ҵ1 ����130 KB
	//	FirstFit(2, 60);	//��ҵ2 ����60 KB
	//	FirstFit(3, 100);	//��ҵ3 ����100 KB
	//	Free(2, 60);		//��ҵ2 �ͷ�60 KB	
	//	FirstFit(4, 200);	//��ҵ4 ����200 KB		
	//	Free(3, 100);		//��ҵ3 �ͷ�100 KB		
	//	Free(1, 130);		//��ҵ1 �ͷ�130 KB		
	//	FirstFit(5, 140);	//��ҵ5 ����140 KB				
	//	FirstFit(6, 60);	//��ҵ6 ����60 KB		
	//	FirstFit(7, 50);	//��ҵ7 ����50 KB		
	//	Free(6, 60);		//��ҵ6 �ͷ�60 KB
	//}
	//else {
	//	BestFit(1, 130);	//��ҵ1 ����130 KB
	//	BestFit(2, 60);	//��ҵ2 ����60 KB
	//	BestFit(3, 100);	//��ҵ3 ����100 KB
	//	Free(2, 60);		//��ҵ2 �ͷ�60 KB	
	//	BestFit(4, 200);	//��ҵ4 ����200 KB		
	//	Free(3, 100);		//��ҵ3 �ͷ�100 KB		
	//	Free(1, 130);		//��ҵ1 �ͷ�130 KB		
	//	BestFit(5, 140);	//��ҵ5 ����140 KB				
	//	BestFit(6, 60);	//��ҵ6 ����60 KB		
	//	BestFit(7, 50);	//��ҵ7 ����50 KB		
	//	Free(6, 60);			//��ҵ6 �ͷ�60 KB
	//}

	cout << "|-------------------------------------------------------|\n"
		<< "|                      ���������㷨                     |\n"
		<< "|-------------------------------------------------------|\n";
	printf("���س�����...");
	getchar();
	option = 1;
	for (int i = 0; i < 11; i++) {  //11������
			
		if (operate[0][i] == 1) { //����
			if (option == 1)
				FirstFit(operate[1][i], p_size[operate[1][i]]);
			else
				BestFit(operate[1][i], p_size[operate[1][i]]);
		}
		else {
			Free(operate[1][i], p_size[operate[1][i]]);
		}
			
	}
	destory(head);	//��������

	cout << "\n\n|-------------------------------------------------------|\n"
		<< "|                      ��������㷨                     |\n"
		<< "|-------------------------------------------------------|\n";
	printf("���س�����...");
	getchar();
	option = 2;
	head = new partition;
	initNode(head);	//���³�ʼ�� 
	for (int i = 0; i < 11; i++) {  //11������

		if (operate[0][i] == 1) { //����
			if (option == 1)
				FirstFit(operate[1][i], p_size[operate[1][i]]);
			else
				BestFit(operate[1][i], p_size[operate[1][i]]);
		}
		else {
			Free(operate[1][i], p_size[operate[1][i]]);
		}

	}
	destory(head);	//��������
	system("pause");

}