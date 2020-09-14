#include "etcp.h"
#define NTIMERS 25
typedef struct tevent_t tevent_t;
struct tevent_t
{
	tevent_t *next;
	struct timeval tv;
	void ( *func )( void * );
	void *arg;
	unsigned int id;
};
static tevent_t *active = NULL;     /* Активные таймеры. */
static tevent_t *free_list = NULL;  /* Неактивные таймеры. */