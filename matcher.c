#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int control_match(char *chunk, char *condition){
    int condition_length = strlen(condition);
    int chunk_length = strlen(chunk);
    int i = 0;

    if(chunk_length != condition_length) return 0;

    while(i < condition_length){
        if( (condition[i]=='v' && is_vowel(chunk[i])) || (condition[i]=='c' && is_consonant(chunk[i])) )
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

char *apply_rule(char *string, rule *r){
	int i = 0, l = strlen(string), rl = strlen(r->result);
	char *final = (char *) malloc((sizeof(char) * rl));

	while(*string){
		if(r->result[i]=='v' || r->result[i]=='c') final[i] = *string;
		else {
			final[i] = '-';
			i++;
			final[i] = *string;
		}
		i++;
		string++;
	}
	return final;
}

char *apply_ruleset(char *string, ruleset rs){
	if(!string) return ".";
	int i = 0, l = strlen(string);
	rule *tmp_rule;
	char *tmp_string = (char *) malloc(sizeof(char) * 3), *final = (char *) malloc(sizeof(char) * l * 2);
	while(i < l){
		strncpy(tmp_string, string, 3);
		if(tmp_rule = match_rule(tmp_string, rs)){
			strcat(final, apply_rule(tmp_string, tmp_rule));
		}
		else {
			strcat(final, string);
			return final;
		}
		string+=3;
		i+=3;
	}
	return final;
}


int main(int argc, char *argv[]){
	ruleset my = NULL;
	insert_rule(&my, create_rule("vcc", "vc-c"));
	insert_rule(&my, create_rule("vcv", "v-cv"));
	insert_rule(&my, create_rule("vvc", "v-vc"));
	insert_rule(&my, create_rule("vvv", "vvv"));
	insert_rule(&my, create_rule("ccv", "c-cv"));
	insert_rule(&my, create_rule("cvc", "cv-c"));
	insert_rule(&my, create_rule("cvv", "cv-v"));
	//printf("%s", argv[1]);
	printf("%s\n", apply_ruleset(argv[1], my));
}
