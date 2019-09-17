#include <archx86.h>
#define LIST_SIZE 64

uint abo_list[LIST_SIZE] = {0};//Liste des variable des aplications abonné
uint last_event[LIST_SIZE] = {0};//Liste des evennements pas encore relevé
uint last_event_p = 0;//Pointeur liste des evennements pas encore relevé

void EventSubscribe(uint var, uint number)//abonne une fonction à un evennement sélectionné.
{
	if (number < LIST_SIZE)
	{
		abo_list[number] = var;
	}
}

void EventRemove(uint number)//retire un evennement à la liste sélectionné.
{
	if (number < LIST_SIZE)
	{
		abo_list[number] = 0;
	}
}

void EventSend(uchar event)//recoit l'IRQ de l'évènnement. Send est du point de vue de l'utilisateur de la fontion
{
	lifo_push(last_event, &last_event_p, LIST_SIZE, (uint)event);//la liste est vide si pointeur = 0
}

uint EventReceive(void)//lance un message à l'application sur l'évenement  renvoi le nombre d'évenement qu'il reste à traiter
{
	uint * var = 0;
	uint event=0;
	if (last_event_p!=0)
	{
		event = lifo_pop(last_event, &last_event_p, LIST_SIZE);
		var = (uint *)abo_list[event];
		*var = event;
		return last_event_p;
	}
	return 0;
}
