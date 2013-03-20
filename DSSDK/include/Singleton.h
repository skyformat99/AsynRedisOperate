/*
	�ļ�	��Singleton.h
	���	��
	����	����־��
	�������ڣ�2009/12/23

	����޸�����	�޸���		�޸�����
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef	_SINGLETON_H_
#define	_SINGLETON_H_

template<class T>
class Singleton
{
public:
	static inline T* GetInstancePtr()
	{
		static T _instance;
		return &_instance;
	}
	
protected:
	Singleton(void){};
	~Singleton(void){};
	Singleton(const Singleton<T>&);
	Singleton<T>& operator=(const Singleton<T>&);

};

#endif
