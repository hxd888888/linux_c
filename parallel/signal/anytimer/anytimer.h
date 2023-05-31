#ifndef ANYTIMER_H__
#define ANYTIMER_H__

#define JOBMAX	 1024

typedef void at_jobfunc_t(void *);

int at_addjob(int sec,at_jobfunc_t *jobp,void *argc);
/*
 *	return  >=  0			  成功，返回任务ID
 *			==  -EINVAL 	  失败，参数非法
 *			==  -ENOSPC		  失败，数组满
 *			==  -ENOMEM		  失败，内存空间不足
 *
 */
int at_canceljob(int id);
/*
 *	return 	==  0			  成功，指定任务已取消
 *			==  -EINVAL		  失败，参数非法
 *			==  -EBUSY		  失败，指定任务已完成
 * 			==  -ECANCELED	  失败，指定任务重复取消
 */
int at_waitjob(int id);
/*
 *	return 	==	0			  成功，指定任务成功释放
 *			==  -EINVAL		  失败，参数非法
 *
 *
 */
at_pausejob();
at_resumejob();
#endif
