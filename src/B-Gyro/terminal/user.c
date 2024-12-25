#include "terminal/_data.h"
#include "terminal/terminal.h"
#include "terminal/tty.h"

#include "klibc/print.h"
#include "klibc/strings.h"

void	initUsers(void){
	_node	*ptr;

	g_users.first = &g_usersNodes[0];
	ptr = g_users.first;
	for (uint8_t i = 0; i < MAX_USERS; i++){
		ptr->ptr = &g_usersData[i];
		ptr->next = &g_usersNodes[i + 1];
		if (i)
			ptr->previous = &g_usersNodes[i - 1];
		ptr = ptr->next;
	}
	g_users.first->previous = &g_usersNodes[MAX_USERS - 1];
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