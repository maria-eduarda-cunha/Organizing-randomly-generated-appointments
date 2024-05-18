#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 200000

typedef struct {
  int Year;
  int Month;
  int Day;
  int Hour;
  int Minute;
  int Duration;
  char Name[100];
} Appointment;

// Print the menu
void menu() {
  printf("\n*MENU*\n1 - Add random input\n2 - Sort by year (Insertion Sort)\n3 - Sort by year (Heap Sort)\n4 - Exit");
  printf("\nChoose an option: ");
}
//////////////////////////////////////

// 1.Add random appointments
void add_Appointment(const char *file, Appointment **appointments, int numLines) {
  FILE *input = fopen(file, "w");
  if (input == NULL) {
    printf("Error opening the file.\n");
    return;
  }
  const char names[5][30] = {"Class", "Thesis", "Webinar", "Meeting", "Presentation"};
  for (int i = 0; i < numLines; i++) {
    (*appointments)[i].Year = (rand() % (2026 - 2010 + 1) + 2010);
      (*appointments)[i].Month = 1 + (rand() % 12);
      (*appointments)[i].Day = 1 + (rand() % 31);
      (*appointments)[i].Hour = (rand() % 23);
      (*appointments)[i].Minute = (rand() % 59);
      (*appointments)[i].Duration = 1 + (rand() % 4);
    strcpy((*appointments)[i].Name, names[(rand() % 5)]);

    // Writes in file
    fprintf(input, "%d;%d;%d;%d;%d;%d;%s\n", (*appointments)[i].Year, (*appointments)[i].Month, (*appointments)[i].Day, (*appointments)[i].Hour, (*appointments)[i].Minute, (*appointments)[i].Duration, (*appointments)[i].Name);
  }

  fclose(input);
}
//////////////////////////////////////

// Swap
void swap(Appointment *a, Appointment *b) {
  Appointment aux = *a;
  *a = *b;
  *b = aux;
}
//////////////////////////////////////

// 2.Sort by year (Insertion Sort)
void insertion_sort(Appointment *appointments, int numLines) {
  clock_t time = clock();
  long int count = 0;

  int j;
  for (int i = 1; i < numLines; i++) {
    count ++;
    int item = appointments[i].Year;
    count ++;
    j = i - 1;
    while (j >= 0 && appointments[j].Year > item) {
      count++;
      swap(&appointments[j], &appointments[j + 1]);
      j--;
      count ++;
    }
    appointments[j + 1].Year = item;
    count ++;
  }

  time = clock() - time;

  printf("\nAlgorithm: Insertion Sort\nInput Size: %d\nExecution Time: %f seconds\nComparisons (steps): %ld\n", numLines, ((float)time)/CLOCKS_PER_SEC, count);
}
//////////////////////////////////////

// 3.Sort by year (Heap Sort)
void heapify(Appointment *appointments, int numLines, int i, long int *count){
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  if (left < numLines && appointments[left].Year > appointments[largest].Year)
    largest = left;
  if (right < numLines && appointments[right].Year > appointments[largest].Year)
    largest = right;
  if (largest != i) {
    swap(&appointments[i], &appointments[largest]);
    heapify(appointments, numLines, largest, count);
    (*count) ++;
  }
}

void heap_sort(Appointment *appointments, int numLines){
  clock_t time = clock();
  long int count = 0;
  for (int i = numLines / 2 - 1; i >= 0; i--)
    heapify(appointments, numLines, i, &count);
  for (int i = numLines - 1; i >= 0; i--) {
    swap(&appointments[0], &appointments[i]);
    heapify(appointments, i, 0, &count);
    count ++;
  }

  time = clock() - time;

  printf("\nAlgorithm: Heap Sort\nInput Size: %d\nExecution Time: %f seconds\nComparisons (steps): %ld\n", numLines, ((float)time)/CLOCKS_PER_SEC, count);
}
//////////////////////////////////////

// Fill output file
void fill_file(Appointment appointments[], int numLines) {
  FILE *output = fopen("output.csv", "w");

  if (output == NULL) {
    printf("Error opening the file.\n");
    return;
  }

  for (int i = 0; i < numLines; i++) {
    fprintf(output, "%d;%d;%d;%d;%d;%d;%s\n", appointments[i].Year, appointments[i].Month, appointments[i].Day, appointments[i].Hour, appointments[i].Minute, appointments[i].Duration, appointments[i].Name);
  }

  fclose(output);
}
//////////////////////////////////////

int main() {
  FILE *input = fopen("input.csv", "w");
  if (input == NULL) {
    printf("Error opening the file.\n");
    return 1;
  }

  Appointment *appointments = malloc(MAX * sizeof(Appointment));
  if (appointments == NULL) {
    printf("Error allocating memory.\n");
    fclose(input);
    return 1;
  }

  Appointment *aux = malloc(MAX * sizeof(Appointment));
  if (aux == NULL) {
      printf("Error allocating memory.\n");
      fclose(input);
      return 1;
  }

  int numLines = 0; 
  while(fscanf(input, "%d;%d;%d;%d;%d;%d;%s", &appointments[numLines].Year, &appointments[numLines].Month, &appointments[numLines].Day, &appointments[numLines].Hour, &appointments[numLines].Minute, &appointments[numLines].Duration, appointments[numLines].Name)!=EOF){
    numLines++;
  }
  fclose(input);


  int option;
  do {
    menu();
    scanf("%d", &option);
    printf("\n");

    for(int i = 0; i < numLines; i++){
       aux[i] = appointments[i]; 
     }

    switch (option) {
      case 1: // Add random input
        printf("Number of appointments: ");
        scanf("%d", &numLines);
        if (numLines > MAX || numLines < 0)
          printf("\nInvalid value.\n");
        else
          add_Appointment("input.csv", &appointments, numLines);
        break;

      case 2: // Sort by year (Insertion Sort)
        if (numLines <= 0)
          printf("Empty input. Add appointments first.\n");
        else {
          insertion_sort(aux, numLines);
          fill_file(aux, numLines);
        }
        break;

      case 3: // Sort by year (Heap Sort)
        if (numLines <= 0)
          printf("Empty input. Add appointments first.\n");
        else {
          heap_sort(aux, numLines);
          fill_file(aux, numLines);
        }
        break;

      case 4: // Exit
        printf("\nExiting program...\n");
        break;

      default:
        printf("Invalid option\n");
        break;
    }
  } while (option != 4);

  free(appointments);

  return 0;
}
