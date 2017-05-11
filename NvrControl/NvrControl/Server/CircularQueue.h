/********************************************* 
Copyright (C), 2014-2038, ORISTARTECH Co., Ltd.
ModuleName ��  ѭ������
FileName   �� CircularQueue.h 
Author     �� xiaozhengxiu@oristartech.com
Date       �� 2014-12-18
Version    �� v1.0
Description�� 
Others     ��
History    �� 
      < Author >    			< Date >             < Modification >
   xiaozhengxiu@oristartech.com        2014-12-18            Add comment
********************************************/  
#ifndef CIRCULAR_QUEUE_H_
#define CIRCULAR_QUEUE_H_

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>  

#define QUEUEDATABSIZE 1024*80 /*8192 */

struct element_Info
{
	unsigned int datalen;
	char* data;//����
};

//template<typename T>

class CircularQueue 
{
public:
//	CircularQueue(const size_t& size) : m_size(size), m_read_index(0), m_write_index(0) {
	CircularQueue(const size_t& size){
		 m_size = size; m_read_index = 0; m_write_index = 0;
		 m_data = new element_Info[m_size];
		for (unsigned int i = 0; i < m_size; i++)//һ���Է��������ڴ�
		{
			m_data[i].datalen = 0;
			m_data[i].data = new char[QUEUEDATABSIZE];
			memset(m_data[i].data,0,QUEUEDATABSIZE);
		}
//       if (!m_data) {// "�洢�ռ����ʧ�ܣ�Ӧ�ó�����ֹ��"
//			exit(1);
//        }
		m_elementNum = 0;
	}

	~CircularQueue(){
		for (unsigned int i = 0; i < m_size; i++)//һ���Է��������ڴ�
		{
			if (m_data[i].data != NULL)
				delete []m_data[i].data;
		}
		if (m_data != NULL)
			delete []m_data;
	  m_read_index = m_write_index = 0;
	  m_elementNum = 0;
	}

	//�ж϶����Ƿ���
	//(m_write_index + 1) % (m_size) ��ʵ��ѭ���Լ� 
	//ѭ�����к��ķ���
	bool IsFull(){  
		if((m_write_index + 1) % (m_size) == m_read_index){  
			return true;  
		} else {  
			return false;  
		}  
	}  

	//���
	bool en_queue(const element_Info& element) {
	  if (IsFull()){  
			printf("en_queue err:queque is full\n");  
		  return false;
		} else {
			memcpy(m_data[m_write_index].data,element.data,element.datalen);
			m_data[m_write_index].datalen = element.datalen;
		    m_write_index = (m_write_index + 1) % m_size;
		    m_elementNum++;
		    return true;
		}
	  }

	//�ж϶����ǲ���Ϊ��.���Ϊ��.�Ͳ��ܳ���  
	bool IsEmpty(){  
		if(m_read_index == m_write_index){  
			return true;  
		} else {  
			return false;  
		}  
	}  

	//����
	bool out_queue(element_Info* element) {
		if (IsEmpty()){  
			printf("out_queue err:queque is empty\n");  
		  return false;
		} else {
			memcpy(element->data,m_data[m_read_index].data,m_data[m_read_index].datalen);
			element->datalen = m_data[m_read_index].datalen;
			memset(m_data[m_read_index].data,0,QUEUEDATABSIZE);
		    m_read_index = (m_read_index + 1) % (m_size);
		    m_elementNum--;
		    return true;
		}
	  }

	// ��ն���
	void clear_queue() {
	  m_read_index = m_write_index = 0;
	  m_elementNum = 0;
	}

	//����Ԫ�ظ���
	int GetElementNum(){
		 return m_elementNum;
	}

	//���д�С
	size_t getQueueSize(void) const{ 
		 return m_size - 1; 
	}

	//��ӡ�������ֵ��ֻ֧������ֵ�����ֻ������  
	void print_queue(){  
		size_t i = m_read_index;  
		while(i != m_write_index){  
			printf("%s ", m_data[i].data);  
			i = (i + 1) % m_size;  
		}  
		printf("\n");
	}  

//��ȡ�ļ���С 
unsigned long long get_file_size(const char *path)  
{  
    unsigned long long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}  

private:
	size_t m_read_index;//ͷָ�룬����ָ��
	size_t m_write_index;//βָ�룬��дָ��
	size_t m_size;//���д�С	
	element_Info* m_data;//����
	unsigned int m_elementNum;//����Ԫ�ظ���
};
#endif

/*
//����ѭ�����к���
int main(int argc, char** argv) {
  CircularQueue circular_queue(6);
  int inum = 0;
  std::string v = "";
  int vlen = 0;
  int i=0;

  v = "a"; 
  element_Info einfo;
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "ab";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abc";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcd";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcde";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcdef";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcdefg";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  element_Info value;
  value.datalen = 0;
  value.data = new char[QUEUEDATABSIZE];
  memset(value.data,0,QUEUEDATABSIZE);

  circular_queue.out_queue(&value);
  printf("���ӣ�--%s--%d--\n", value.data,value.datalen);
  memset(value.data,0,QUEUEDATABSIZE);
  inum = circular_queue.GetElementNum();
   printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

   circular_queue.out_queue(&value);
  printf("���ӣ�--%s--%d--\n", value.data,value.datalen);
  memset(value.data,0,QUEUEDATABSIZE);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  circular_queue.out_queue(&value);
  printf("���ӣ�--%s--%d--\n", value.data,value.datalen);
  memset(value.data,0,QUEUEDATABSIZE);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcdefgh";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcdefghi";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcdefghij";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcdefghijk";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  circular_queue.out_queue(&value);
  printf("���ӣ�--%s--%d--\n", value.data,value.datalen);
  memset(value.data,0,QUEUEDATABSIZE);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  v = "abcdefghijkl";
  einfo.data = (char*)v.c_str();
  einfo.datalen = (unsigned int)v.length();
  circular_queue.en_queue(einfo);
  inum = circular_queue.GetElementNum();
  printf("����Ԫ�ظ�����%d\n",inum);
  circular_queue.print_queue();

  printf("���д�С��%d\n",circular_queue.getQueueSize());

  if (value.data != NULL)
	 delete []value.data;

  return 0;
}*/


