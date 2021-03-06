#include<time.h>
#include<xen/xen.h>

extern shared_info_t *shared_info;

uint64_t tscToNanoseconds(uint64_t tsc,struct vcpu_time_info *timeinfo)
{
	return (tsc<<timeinfo->tsc_shift)*timeinfo->tsc_to_system_mul;
}

#define NANOSECONDS(tsc) (tsc<<shared_info->cpu_info[0].time.tsc_shift)	\
	* shared_info->cpu_info[0].time.tsc_to_system_mul;
	
#define RDTSC(x) asm volatile ("RDTSC" : "=A"(tsc))

int gettimeofday(struct timeval *tp,struct timezone *tzp)
{
	uint64_t tsc;
	/*GET THE TIME VALUE FROM THE SHARED INFO PAGE*/
	uint32_t version,wc_version;
	uint32_t seconds,nanoseconds,system_time;
	uint64_t old_tsc;
	/*LOOP UNTIL WE CAN READ ALL REQUIRED VALUES FROM THE SAME UPDATE*/
	do
	{
		/*SPIN IF THE ITME VALUE IS BEING UPDATED*/
		do
		{
			wc_version=shared_info->wc_version;
			version=shared_info->cpu_info[0].time.version;
		}while(
			version&1==1
			||
			wc_version&1==1);
		/*READ THE VALUES*/
		seconds=shared_info->wc_sec;
		nanoseconds=shared_info->wc_nsec;
		system_time=shared_info->cpu_info[0].time.system_time;
		old_tsc=shared_info->cpu_info[0].time.tsc_timestamp;
	}while(
		version!=shared_info->cpu_info[0].time.version
		||
		wc_version!=shared_info->wc_version
	);
	/*GET THE CURRENT TSC VALUE*/
	RDTSC(tsc);
	/*GET THE NUMBER OF ELAPSED CYCLES*/
	tsc-=old_tsc;
	/*UPDATE THE SYSTEM TIME*/
	system_time+=NANOSECONDS(tsc);
	/*UPDATE THE NANOSECOND TIME*/
	nanoseconds+=system_time;
	/*MOVE COMPLETE SECONDS TO THE SECOND COUNTER*/
	seconds+=nanoseconds/1000000000;
	nanoseconds=nanoseconds%1000000000;
	/*RETURN THE SECONDS AND MILISECONDS VALUE*/
	tp->tv_sec=seconds;
	tp->tv_usec=nanoseconds*1000;
	return 0;
}
