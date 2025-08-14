#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max-Queue 50
#define Max-Spots 5
#define Max-Name 20

typedef struct Event {
    int id;
    char name[Max-Name];
    char date[11];
    char spot[10];
    struct Event* next;
} Event;

Event* events = NULL;

int queue[Max-Queue];
int q_front = 0, q_rear = -1, q_size = 0;

typedef struct Student {
    int id;
    struct Student *left, *right;
} Student;

Student* students = NULL;

int map[Max-Spots][Max-Spots];

void add(int id, char* name, char* date, char* spot) {
    Event* new_event = (Event*)malloc(sizeof(Event));
    new_event->id = id;
    strncpy(new_event->name, name, Max-Name - 1);
    new_event->name[Max-Name - 1] = '\0';
    strncpy(new_event->date, date, 10);
    new_event->date[10] = '\0';
    strncpy(new_event->spot, spot, 9);
    new_event->spot[9] = '\0';
    new_event->next = events;
    events = new_event;
    printf("Added event: %s\n", name);
}

void show_events() {
    Event* curr = events;
    printf("Events:\n");
    while (curr) {
        printf("ID: %d, %s, %s, %s\n", curr->id, curr->name, curr->date, curr->spot);
        curr = curr->next;
    }
}

void free_events() {
    Event* curr = events;
    while (curr) {
        Event* temp = curr;
        curr = curr->next;
        free(temp);
    }
}

void signup(int id) {
    if (q_size < Max-Queue) {
        queue[++q_rear] = id;
        q_size++;
        printf("Student %d signed up\n", id);
    } else {
        printf("Queue full!\n");
    }
}

int process() {
    if (q_size > 0) {
        int id = queue[q_front++];
        q_size--;
        printf("Processed student: %d\n", id);
        return id;
    }
    printf("Queue empty\n");
    return -1;
}

int peek() {
    if (q_size > 0) return queue[q_front];
    return -1;
}

void show_queue() {
    printf("Queue:\n");
    for (int i = q_front; i <= q_rear; i++)
        printf("Student ID: %d\n", queue[i]);
}

Student* new_student(int id) {
    Student* node = (Student*)malloc(sizeof(Student));
    node->id = id;
    node->left = node->right = NULL;
    return node;
}

void add_student(int id) {
    Student** curr = &students;
    while (*curr) {
        if (id < (*curr)->id) curr = &(*curr)->left;
        else if (id > (*curr)->id) curr = &(*curr)->right;
        else return;
    }
    *curr = new_student(id);
    printf("Added student %d\n", id);
}

Student* find_student(int id) {
    Student* curr = students;
    while (curr) {
        if (id == curr->id) return curr;
        else if (id < curr->id) curr = curr->left;
        else curr = curr->right;
    }
    return NULL;
}

void free_students(Student* node) {
    if (node) {
        free_students(node->left);
        free_students(node->right);
        free(node);
    }
}

void setup_map() {
    int paths[Max-Spots][Max-Spots] = {
        {0, 1, 1, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1},
        {0, 1, 1, 0, 1},
        {0, 0, 1, 1, 0}
    };
    for (int i = 0; i < Max-Spots; i++)
        for (int j = 0; j < Max-Spots; j++)
            map[i][j] = paths[i][j];
}

void find_path(int start, int end) {
    int visited[Max-Spots] = {0}, parent[Max-Spots], q[Max-Spots];
    int front = 0, rear = -1;
    q[++rear] = start;
    visited[start] = 1;
    parent[start] = -1;
    while (front <= rear) {
        int curr = q[front++];
        if (curr == end) break;
        for (int i = 0; i < Max-Spots; i++)
            if (map[curr][i] && !visited[i]) {
                q[++rear] = i;
                visited[i] = 1;
                parent[i] = curr;
            }
    }
    if (!visited[end]) {
        printf("No path found\n");
        return;
    }
    printf("Path: ");
    int path[Max-Spots], len = 0, curr = end;
    while (curr != -1) {
        path[len++] = curr;
        curr = parent[curr];
    }
    char* spots[] = {"Library", "Cafe", "Hall", "Lab", "Dorm"};
    for (int i = len - 1; i >= 0; i--)
        printf("%s%s", spots[path[i]], i > 0 ? " -> " : "\n");
}

int main() {
    setup_map();
    int choice, id, start, end;
    char name[Max-Name], date[11], spot[10];
    while (1) {
        printf("\n=== Campus App ===\n1. Add Event\n2. Show Events\n3. Sign Up\n4. Show Queue\n5. Process Sign-Up\n6. Add Student\n7. Find Student\n8. Find Path\n9. Exit\nChoose: ");
        scanf("%d", &choice);
        if (choice == 9) break;
        switch (choice) {
            case 1:
                printf("Enter ID, Name, Date (YYYY-MM-DD), Spot: ");
                scanf("%d %s %s %s", &id, name, date, spot);
                add(id, name, date, spot);
                break;
            case 2:
                show_events();
                break;
            case 3:
                printf("Enter Student ID: ");
                scanf("%d", &id);
                signup(id);
                break;
            case 4:
                show_queue();
                break;
            case 5:
                process();
                break;
            case 6:
                printf("Enter Student ID: ");
                scanf("%d", &id);
                add_student(id);
                break;
            case 7:
                printf("Enter Student ID: ");
                scanf("%d", &id);
                printf(find_student(id) ? "Student found!\n" : "Student not found\n");
                break;
            case 8:
                printf("Enter start & end (0=Library, 1=Cafe, 2=Hall, 3=Lab, 4=Dorm): ");
                scanf("%d %d", &start, &end);
                find_path(start, end);
                break;
            default:
                printf("Wrong choice!\n");
        }
    }
    free_events();
    free_students(students);
    return 0;
}