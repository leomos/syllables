#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CONDITION_LENGTH 3
struct _rule {
	char *condition;
	char *result;
};
typedef struct _rule rule;

struct _ruleset_element {
		rule *r;
		struct _ruleset_element *next;
};
typedef struct _ruleset_element ruleset_element;

typedef ruleset_element *ruleset;

rule *create_rule(char *condition, char *result){
	rule *obj = (rule *) malloc(sizeof(rule));
	obj->condition = strdup(condition);
	obj->result = strdup(result);
	return obj;
}

void insert_rule(ruleset* rs, rule *ru) {
    ruleset tmp=(ruleset)malloc(sizeof(ruleset_element));
	tmp->r = (rule *) malloc(sizeof(rule));
    tmp->r->condition = ru->condition;
    tmp->r->result = ru->result;
	tmp->next=*rs;
    *rs=tmp;
}

void print_ruleset(ruleset rs) {
    if(rs==NULL) return;
    else {
        printf("%s -> %s\n", rs->r->condition, rs->r->result);
        print_ruleset(rs->next);
    }
}


int is_vowel(char c) {
	return (c=='a' || c=='e' || c=='i' || c=='o' || c=='u' ||
			c=='A' || c=='E' || c=='I' || c=='O' || c=='U');
}

int is_consonant(char c){
	return ((c > 'a') && (c <= 'z')) || ((c>'A') && (c <= 'Z')) && !is_vowel(c);
}

int is_letter(char c){
	return is_vowel(c) || is_consonant(c);
}

int is_valid_symbol(char c){
	return c=='@' || c=='#';
}

int is_valid_rule_symbol(char c){
	return is_letter(c) || is_valid_symbol(c);
}

int control_match(char *chunk, char *condition){
    int condition_length = strlen(condition);
    int chunk_length = strlen(chunk);
    int i = 0;

    if(chunk_length != condition_length) return 0;

    while(i < condition_length){
        if( ((condition[i]=='@' && is_vowel(chunk[i])) || (condition[i]=='#' && is_consonant(chunk[i]))) || (chunk[i]==condition[i]) )
            i++;
        else
            return 0;
    }
    return 1;
}

rule *match_rule(char *string, ruleset rs){
	if(rs == NULL) return NULL;
	else {
		if(control_match(string, rs->r->condition)) return rs->r;
		else match_rule(string, rs->next);
	}
}

char *apply_rule(char *chunk, rule *rule_to_be_applied){
	int rule_result_iterator = 0;
	int rule_result_length = strlen(rule_to_be_applied->result);
	char *evaluated_chunk = (char *) malloc(sizeof(char) * rule_result_length);

	while(rule_result_iterator < rule_result_length) {
		if(is_valid_rule_symbol(rule_to_be_applied->result[rule_result_iterator])){
			evaluated_chunk[rule_result_iterator] = *chunk;
			chunk++;
		} else {
			evaluated_chunk[rule_result_iterator] = rule_to_be_applied->result[rule_result_iterator];
		}
		rule_result_iterator++;
	}
	return evaluated_chunk;
}

char *apply_ruleset(char *matching_string, ruleset matching_ruleset){
	/*
	 * DEFS:
	 * chunk = group of DEFAULT_CONDITION_LENGTH characters that needs to be matched
	 *
	*/
	if(!matching_string || !matching_ruleset) return "."; // If one of the two arguments is NULL, returns a string with just a point "."

	rule *current_matching_rule; // The rule that is going to be used for chunk of string that needs to be evaluated and matched.

	char *temporary_string = (char *) malloc(sizeof(char) * DEFAULT_CONDITION_LENGTH); //the temporary string that will contain
																					   //the chunk that needs to be matched
	int matching_string_length = strlen(matching_string);

	char *evaluated_result =  (char *) malloc(sizeof(char) * matching_string_length * 2);

	int chunk_iterator = 0;

	/*
	 * For every chunk a rule is applied if matching
	*/
	while(chunk_iterator < matching_string_length){
		strncpy(temporary_string, matching_string, DEFAULT_CONDITION_LENGTH);

		if(current_matching_rule = match_rule(temporary_string, matching_ruleset)){
			strcat(evaluated_result, apply_rule(temporary_string, current_matching_rule));
		} else {
			strcat(evaluated_result, matching_string);
		}

		current_matching_rule = NULL;
		matching_string += DEFAULT_CONDITION_LENGTH;
		chunk_iterator += DEFAULT_CONDITION_LENGTH;
	}
	free(temporary_string);
	free(current_matching_rule);
	return evaluated_result;
}

void benchmark(char *file_path, ruleset rs){
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
		printf("%s", apply_ruleset(line, rs));
    }
    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
	ruleset my = NULL;

	/*
	 * General rules
	*/
	insert_rule(&my, create_rule("@##", "@#-#"));
	insert_rule(&my, create_rule("@#@", "@-#@-"));
	insert_rule(&my, create_rule("@@#", "@-@#-"));
	insert_rule(&my, create_rule("@@@", "@@@"));
	insert_rule(&my, create_rule("##@", "#-#@-"));
	insert_rule(&my, create_rule("#@#", "#@-#"));
	insert_rule(&my, create_rule("#@@", "#@-@"));

	/* Bunch of few particular rules just for fun
	*/
	insert_rule(&my, create_rule("fra", "fra-"));
	insert_rule(&my, create_rule("aer", "a-e-r"));
	insert_rule(&my, create_rule("eo#", "e-o-#"));
	insert_rule(&my, create_rule("tin", "tin-"));


	printf("%s\n", apply_ruleset(argv[1], my));
}
