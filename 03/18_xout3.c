#define ACK		0x6		/* Символ подтверждения ACK. */
#define MRSZ	128		/* Максимальное число неподтвержденных сообщений. */
#define T1		3000	/* ждать 3 с до первого ACK */
#define T2		5000	/* и 5 с до второго ACK. */
#define ACKSZ ( sizeof( u_int32_t ) + 1 )
typedef struct			/* Пакет данных. */
{
	u_int32_t len;		/* Длина признака и данных. */
	u_int32_t cookie;	/* Признак сообщения. */
	char buf[ 128 ];	/* Сообщение. */
} packet_t;
typedef struct			/* Структура сообщения. */
{
packet_t pkt;			/* Указатель на сохранённое сообщение. */
int id;					/* Идентофикатор таймера. */
} msgrec_t;
static msgrec_t mr[ MRSZ ];
static SOCKET s;