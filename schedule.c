 /* Name: Sakshat Bhattarai
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"

/* Schedule Count
 * - Return the number of Processes in the list.
 */
int schedule_count(Process *list) {
  if(list == NULL){
  	return 0;
  }
  int count = 0;
  Process *walker = list;
  //count the no of nodes by iterating over the list
  while (walker != NULL){
  	count++;
  	walker = walker->next;
  }
  return count;
}


/* Schedule Insert
 * - Insert the Process into the List with the Following Constraints
 *   list is a pointer to the head pointer for a singly linked list.
 *   It may begin as NULL
 *   1) Insert in the order of Ascending PIDs
 *   (eg. 1, 13, 14, 20, 32, 55)
 * Remember to set list to the new head if it changes!
 */
void schedule_insert(Process **list, Process *node) {
  //check if the node is NULL
  if(node == NULL){
  	return;
  }
  //if list is empty, start the list with the node
  if(*list == NULL){
  	*list = node;
  	return;
  }
  //check if node is to be inserted at the front
  if(node->pid < (*list)->pid){
  	node->next = *list;
  	*list = node;
  	return;
  }
  //check where to install the node if not in front
  Process *walker = *list;
  while(walker != NULL && walker->next != NULL){
  	if(walker->next->pid >= node->pid){
  		node->next = walker->next;
  		walker->next = node;
  		return;
  	}
  	walker = walker->next;
  }
  //insert the node at last
  walker->next = node;
  return;
}


/* Schedule Terminate
 * - Unallocate the Memory for the given Node
 */
void schedule_terminate(Process *node) {
  if(node == NULL){
  	return;
  }
  //set the address pointed to by the node to NULL and free it
  node->next = NULL;
  free(node);
  return;
}


/* Schedule Generate
 * - Allocate the Memory and Create the Process from the given Variables
 *   Remember to initialize all values, including next!
 */
Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) {
  //assign a new pointer to a memory address
  Process *new = malloc(sizeof(Process));
  //check for memory errors
  if(new == NULL){
  	return NULL;
  }
  //if fine, assign values to the new node
  strncpy(new->name, name, strlen(name)+1);
  new->pid = pid;
  new->time_remaining = time_remaining;
  new->time_last_run = time_last_run;
  new->next = NULL;
  return new;
}


/*function definition and declaration for low_time_r function
  This function selects the process with lowest time_remaining
  it is called from schedule_select()
*/
Process *low_time_r(Process *walk){
  Process *low_time_p = walk;
  while(walk->next != NULL){
    //if time_remaining is lower, select it
    if(walk->next->time_remaining < low_time_p->time_remaining){
      low_time_p = walk->next;
    }
    //if time_remaining is equal, select the one with lowest pid
    else if(walk->next->time_remaining == low_time_p->time_remaining){
      if(walk->next->pid < low_time_p->pid){
        low_time_p = walk->next;
      }
    }
    walk = walk->next;
  }
  return low_time_p;
}


/* Schedule Select
 * - Select the next Process to be run using the following conditions:
 *   1) The Process with the lowest time_remaining should run next.
 *   - If there are any ties, select the Process with the lowest PID
 *   2) If there is a Process that has not run in >= TIME_STARVATION, then
 *      select that one instead.
 *      (Use the function clock_get_time() to get the current time)
 *   - If there are any ties, select the Process with the lowest PID
 *   3) If the list is NULL, return NULL
 * - Remove the selected Process from the list
 * - Return the selected Process
 */
Process *schedule_select(Process **list) {
  if(*list == NULL){
    return NULL;
  }
  Process *walk = *list;
  Process *select = NULL;
  //select the process with lowest time_remaining
  select = low_time_r(walk);
  //select the process with time_starvation
  //iterate over the list, the first node will have lowest pid
  walk = *list;
  while(walk->next != NULL){
    int timenr = clock_get_time() - walk->time_last_run;
    if(timenr >= TIME_STARVATION){
      select = walk;
    }
    walk = walk->next;
  }
  //remove the node from the list
  //if the first node is selected
  if(select == *list){
    *list = select->next;
    return select;
  }
  //if any other node is selected
  else{
    walk = *list;
    while(walk->next != select){
      walk = walk->next;
    }
    walk->next = select->next;
    return select;
  }
}
