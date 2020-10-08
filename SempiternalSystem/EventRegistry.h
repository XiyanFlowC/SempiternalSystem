#pragma once
#include <cstdlib>

//�¼�������ע���������
class EventRegistry
{
protected:
	//�Ѿ�ע��ĺ���
	void (** registeredFunc) (int, void*);
	//ע������
	int funcSize;

	struct RegistryTerm {
		int key;
		int index;
	} *terms;
public:
	//�Ѿ�ע��ĺ�������
	int size;

	//���¼�ע��һ���º���
	//�ɹ�����0,ʧ�ܷ��ط�0. �ر��,ע�������������-1
	//p_func: ����
	//key: ����
	virtual int Register(void (*p_func) (int, void*), int key);

	//ע��һ������(���Ƽ�)
	//p_func: Ҫע��ĺ���
	virtual int Register(void(*p_func)(int, void*));

	//��������ʹ���¼�
	virtual void HandleEvent(int p_key, int argc, void* args);

	//ִ��ע����к���
	virtual void ExecFunc(int p_index, int argc, void* args);

	//��Ĭ��������128����ʼ���¼�ע���
	EventRegistry();

	//��ָ����������ʼ���¼�ע���
	EventRegistry(int p_size);

	~EventRegistry();
};

