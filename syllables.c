/*
		text: |
			Scrivere una funzione `sillaba` che, presa in
			ingresso una stringa,
			la divida in sillabe. La regola è: se la parola
			è del tipo "cvcvcv...." (dove 'c' rappresenta una consonante e 'v' una vocale) , i gruppi 'cv' formano una sillaba.
			Se la parola è del tipo 'vcvcvcv....' la prima vocale fa
			sillaba a sé, e per le altre vale la regola di sopra
		esempi:
			- e-va-de-re
			- a-si-no
			- ca-sa
			- di-va-ga-va-no
		test: |
			assert(strcmp("evadere","e-va-de-re")==0)
			assert(strcmp("asino,"a-si-no")==0)
			assert(strcmp("casa","ca-sa")==0)
*/


#include "lib/dict.h"

int is_vowel(char c) {
	return (c=='a' || c=='e' || c=='i' || c=='o' || c=='u' ||
			c=='A' || c=='E' || c=='I' || c=='O' || c=='U');
}

int is_consonant(char c){
	return ((c > 'a') && (c <= 'z')) || ((c>'A') && (c <= 'Z')) && !is_vowel(c);
}

char *extract_first_syllable_starting_from_vowel(char *chunk){
	char *result = (char *)malloc(sizeof(char) * 2);
	result[0] = chunk[0];
	result[1] = '\0';
	return result;
}
char *extract_first_syllable_starting_from_consonant(char *chunk){
	char *result = (char *)malloc(sizeof(char) * 3);
	result[0] = chunk[0];
	result[1] = chunk[1];
	result[2] = '\0';
	return result;
}

char *extract_first_syllable(char *chunk){
	if(is_vowel(*chunk)){
		return extract_first_syllable_starting_from_vowel(chunk);
	} else {
		return extract_first_syllable_starting_from_consonant(chunk);
	}
}

char *syllablize(char *string){
	char *result = (char *) malloc(sizeof(char) * strlen(string) * 2);
	char *syllable = (char *) malloc(sizeof(char) * 2);
	while(*string){
		syllable = extract_first_syllable(string);
		strcat(result, syllable);
		strcat(result, "-");
		string += strlen(syllable);
	}
	result[strlen(result)-1] = '\0';
	return result;
}

elem *store_syllables_ordered(char *string){
	elem *head = NULL;
	char *token, *tmp = syllablize(string);
	while((token = strsep(&tmp, "-")) != NULL){
		inc(&head, token);
	}
	return head;
}

int main(){
	char *s = strdup("divagavano");
	print_dict(store_syllables_ordered(s));
}
