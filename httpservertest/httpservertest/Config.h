#ifndef CONFIG_H
#define CONFIG_H

enum RWLOCK
{
	RWLOCK_FREE,
	RWLOCK_READ,
	RWLOCK_WRITE
};

class NoCopyable
{
private:
	NoCopyable(const NoCopyable&);
	NoCopyable& operator=(const NoCopyable&);
public:
	NoCopyable();
};


#endif