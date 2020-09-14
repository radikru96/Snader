#ifndef __HEARTBEAT_H__
#define __HEARTBEAT_H__
#define MSG_TYPE1		1	/* Сообщение прикладного уровня. */
#define MSG_TYPE2		2	/* Ещё одно. */
#define MSG_HEARTBEAT	3	/* Сообщение-пульс. */
typedef struct				/* Структура сообщения. */
{
u_int32_t type;				/* MSG_TYPE1, ... */
char data[ 2000 ];
} msg_t;
#define T1			60	/* Время простоя перед отправкой пульса. */
#define T2			10	/* Время ожидания ответа. */
#endif  /* __HEARTBEAT_H__ */