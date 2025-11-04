#include "terminal/_data.h"
#include "terminal/terminal.h"
#include "terminal/tty.h"

#include "klibc/print.h"
#include "klibc/strings.h"
#include "klibc/listUtils.h"
#include "memory/malloc/malloc.h"

void	initUsers(void){
	_node	*ptr;

	for (uint8_t i = 0; i < MAX_USERS; i++){
		ptr = (_node *)calloc(sizeof(_node *), 1);
		insertNodeInList(&g_users, ptr);
		ptr->ptr = (_user *)calloc(sizeof(_user *), 1);
		((_user *)(ptr->ptr))->id = i;
	}

	makeItCircularList(&g_users);

	g_users.last = g_users.first;
	g_users.current = NULL;

	g_users.size = 1;

	strlcpy(((_user *)g_users.first->ptr)->username, "root", 4);
	strlcpy(((_user *)g_users.first->ptr)->password, " ", 1);
	((_user *)g_users.first->ptr)->id =  g_id++;
}

_node	*getUserID(char *username){
	_node	*ptr;

	ptr = g_users.first;
	for (uint32_t i = 0; i < g_users.size; i++){
		if (!strcmp(username, ((_user *)ptr->ptr)->username))
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}

uint8_t	checkUser(char *user, char *pass){
	_node	*ptr;
	_user	*tmp;

	ptr = g_users.first;
	for (uint32_t i = 0; i < g_users.size; i++){
		tmp = ptr->ptr;
		if (!strcmp(user, tmp->username) \
			&& !strcmp(pass, tmp->password)) {
				g_users.current = ptr;
				return (1);
		}
		ptr = ptr->next;
	}
	return (0);
}