#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Task{
	char description[200];
	int status;
};

struct Task Tasks[500];
int Taskslength = 500;
int Index = 0; //index of the first available place in tasks list

int loadTasks(){
	printf("loading Tasks..\n");
	FILE * TaskFile = fopen("Tasks.txt", "rb");
	if (TaskFile == NULL){
		printf("Error while opening file");
		return 1;
	};
	while (fread(&(Tasks[Index]), sizeof(struct Task), 1, TaskFile)){
	   if (Index == Taskslength-1){
	   	break;
	   };
	   Index ++;
    };
    if (fclose(TaskFile) != 0){
    	printf("error: File didnt close successfully \n");
    };
};

void SaveTasks(){
	FILE * TaskFile = fopen("Tasks.txt", "wb");
	for (int i=0; i<Index; i++){
		fwrite(&(Tasks[i]), sizeof(struct Task), 1, TaskFile);
	}
	if (fclose(TaskFile) != 0){
    	printf("error: File didnt close successfully\n");
    };
};

void AddTask(){
    char description[200];
    char exitPhrase[] = "esc";
    
    printf("\n                   ______Add Task______");
    printf("\n        (Enter word 'esc' instead of task to exit mode\n");
    
    while(1){
	    printf("\nEnter Task description: ");
	    fgets(description, sizeof(description), stdin);
	    description[strcspn(description, "\r\n")] = '\0';
	    if (strcmp(description, exitPhrase) == 0){
	    	break;
	    };
	    struct Task new_task;
	    strcpy(new_task.description, description);
	    new_task.status = 1;
	    if (Index == Taskslength-1){
	    	printf("Tasks limit exceeded");
	    	break;
	   }
	    Tasks[Index] = new_task;
	    Index ++;
	    
	    printf("Task Added Successfully\n");
	    
    }
};

void ViewTasks(){
	
	printf("\n                   ______All Tasks_____\n\n");
	if (Index == 0){
		printf("                   {No Tasks Available} \n");
	};
	
	char status[10];
	for (int i=0; i<Index; i++) {
		if (Tasks[i].status == 1){
			strcpy(status,"Pending...");
		}else{
			strcpy(status,"Done");
		}
		printf("Description: %s | Status: %s\n", Tasks[i].description, status);
	};
};

void DeleteTasks(){
	char description[200];
	
	char exitPhrase[] = "esc";
	printf("\n                   ______Delete Task______");
	printf("\n        (Enter word 'esc' instead of task to exit mode\n");
	
	while(1){
		printf("\nSearch Task: ");
		fgets(description, sizeof(description), stdin);
		description[strcspn(description, "\r\n")] = '\0';
		
		if (strcmp(description, exitPhrase) == 0){
	    	break;
	    };
		
		int tempindex = 0;
	    char choice[5];
	    int resultIndices[Index];
	    
	    printf("\n Matching Tasks:- \n");
	    printf("(Choose with number)\n");
	    for (int i=0; i < Index; i++){
	    	if (strstr(Tasks[i].description, description)){
	    		printf("    %d . %s\n", tempindex+1, Tasks[i].description);
	    		resultIndices[tempindex] = i;
	    		tempindex ++;
	    	};
	    };
	    if (tempindex == 0){
	    	printf("    {No matching tasks}");
	    	break;
	    };
	    printf("    Enter Choice: ");
	    fgets(choice, sizeof(choice), stdin);
	    choice[strcspn(choice, "\r\n")] = '\0';
	    int i = resultIndices[atoi(choice)-1];
	    for (int j = i; j < Index; j++){
	    	Tasks[j] = Tasks[j+1];
	    }
	    printf("    Task Deleted Successfully\n");
	};
};

int SwitchStatus(){
	if (Index == 0){
		printf("                  {No Tasks Available}\n");
		return 0;
	};
	char description[200];
    char exitPhrase[] = "esc";
    printf("\n                   ______Switch Status______");
    printf("\n        (Enter word 'esc' instead of task to exit mode\n\n");
    
    while(1){
	    printf("Search tasks: ");
	    fgets(description, sizeof(description), stdin);
	    description[strcspn(description, "\r\n")] = '\0';
	    if (strcmp(description, exitPhrase) == 0){
	    	break;
	    };
	    int tempindex = 0;
	    char choice[5];
	    int resultIndices[Index];
	    
	    printf("\n Matching Tasks:- \n");
	    printf("(Choose with number)\n");
	    for (int i=0; i < Index; i++){
	    	if (strstr(Tasks[i].description, description)){
	    		printf("    %d . %s\n", tempindex+1, Tasks[i].description);
	    		resultIndices[tempindex] = i;
	    		tempindex ++;
	    	};
	    };
	    if (tempindex == 0){
	    	printf("    {No matching tasks}");
	    	return 0;
	    };
	    printf("    Enter Choice: ");
	    fgets(choice, sizeof(choice), stdin);
	    choice[strcspn(choice, "\r\n")] = '\0';
	    Tasks[resultIndices[atoi(choice)-1]].status = Tasks[resultIndices[atoi(choice)-1]].status * -1;
	    printf("    Status Switched successfully \n");
    };
};

int main() {
    printf("               _______Task Manager_______\n ");
    printf("                (Enter number to select)\n\n");
    printf("1. Add Task\n2. View Tasks\n3. Delete Task\n4. Switch Status\n5. Escape\n\n");
    loadTasks();
  
    while (1){
	    char mode[5];
	    int num;
	    printf("                 _____Mode Selection_____\n");
	    printf("\nEnter number: ");
	    fgets(mode, sizeof(mode), stdin);
	    mode[strcspn(mode, "\r\n")] = '\0';
	    num = atoi(mode);
	    
	    if (num == 5){
	    	break;
	    }
	    switch (num){
	        case 1:
	            AddTask();
	            break;
	        case 2:
	        	ViewTasks();
	        	break;
	        case 3:
	        	DeleteTasks();
	        	break;
	        case 4:
	        	SwitchStatus();
	        	break;
	        default:
	        	printf(" Invalid Mode Entered ");
	    };
	    printf("\n");
    };
    SaveTasks();
    return 0;
};