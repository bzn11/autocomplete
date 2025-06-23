#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"


int compare_terms(const void *a, const void *b) {
    return strcmp(((term *)a)->term, ((term *)b)->term);
}

int compare_weight(const void *a, const void *b) {
    double diff = ((term *)b)->weight - ((term *)a)->weight;
    if(diff > 0)
        return 1;
    else if(diff < 0)
        return -1;
    else
        return 0;
}

void read_in_terms(term **terms, int *pnterms, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    char temp[200];
    if (fgets(temp, sizeof(temp), file) == NULL) {
        printf("Error reading first line\n");
        fclose(file);
        return;
    }

    int n = 0;
    while (fgets(temp, sizeof(temp), file) != NULL) {
        n++;
    }
    *pnterms = n;

    *terms = malloc(n * sizeof(term));
    if (*terms == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        return;
    }

    rewind(file);
    fgets(temp, sizeof(temp), file);

    for (int i = 0; i < n; i++) {
        fscanf(file, "%lf\t%[^\n]", &(*terms)[i].weight, (*terms)[i].term);
    }

    fclose(file);

    qsort(*terms, n, sizeof(term), compare_terms);
}




int lowest_match(term *terms, int nterms, char *substr) {
    int low = 0, high = nterms - 1;
    int match_index = -1;
    int substr_len = strlen(substr);
    
    while(low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strncmp(terms[mid].term, substr, substr_len);
        if(cmp == 0) {
            match_index = mid;
            high = mid - 1;  
        } else if(cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return match_index;
}

int highest_match(term *terms, int nterms, char *substr) {
    int low = 0, high = nterms - 1;
    int match_index = -1;
    int substr_len = strlen(substr);
    
    while(low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strncmp(terms[mid].term, substr, substr_len);
        if(cmp == 0) {
            match_index = mid;  
            low = mid + 1; 
        } else if(cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return match_index;
}


void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr) {
    int low_index = lowest_match(terms, nterms, substr);
    int high_index = highest_match(terms, nterms, substr);
    
    if(low_index == -1 || high_index == -1) {
        *n_answer = 0;
        *answer = NULL;
        return;
    }
    
    *n_answer = high_index - low_index + 1;
    
    *answer = (term *)malloc((*n_answer) * sizeof(term));
    if(*answer == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    
    int i;
    for(i = 0; i < *n_answer; i++) {
        (*answer)[i] = terms[low_index + i];
    }
        qsort(*answer, *n_answer, sizeof(term), compare_weight);
}