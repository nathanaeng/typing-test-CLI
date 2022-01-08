#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#define GET_TIME(__TIME_now) {		              					\
    struct timeval t;									\
    gettimeofday(&t, NULL);								\
    __TIME_now = (double)(t.tv_sec + (t.tv_usec/1e6));	                                \
    }										        \

/* WORD LIST */
char* words[] = {"a", "at", "an", "any", "another", "ask", "as", "art", "aid", "and", "also", "application", "ancient", "ant",
                "but", "be", "besides", "brother", "buy", "by", "brought", "bring",
                "can", "cat", "cowardly", "count", "case", "chair", "clear", "capital", "cloud", "coat", "credit",
                "direct", "drill", "dog", "down", "do", "drain", "difficult", "ditch", "diet", "does",
                "ethical", "eye", "eat",
                "flame", "frog", "flower", "failure", "freedom", "flask", "fox",
                "goat", "grain", "go", "glad", "gift",
                "hard", "hail", "happy", "hat", "hit", "he", "him", "her", "his", "hers", "how",
                "illustrate", "insult", "it", "ill", "in", "I",
                "jail", "jet", "jab", "jar", "jealous",
                "know", "kind", "kale", "kid", "knit",
                "love", "lose", "let", "long", "loan", "list",
                "mountain", "move", "more", "mouth", "mist", "mile", "metric", "mail", "must",
                "no", "not", "nice",
                "of", "oil", "or", "orchestra", "oatmeal", "oak", "original", "official", "open",
                "princess", "pretty", "polite", "pet", "pillow", "people", "pit", "pile",
                "quilt", "quiet", "quit", "quite",
                "read", "random", "red", "row", "rip", "rude", "respect", "run", "robot",
                "silent", "so", "sat", "sit", "symptom", "sad", "sell", "sick", "slide",
                "treasure", "treat", "trim", "to", "the", "there", "thus", "their", "talent", "trail",
                "up", "under", "uncle", "urge", "utilize", "use", "utter", "ultimate", "understand",
                "violent", "vet", "vampire", "village", "vote", "vile",
                "wet", "we", "weather", "while", "who", "whim", "walk", "what", "will",
                "xany",
                "yes", "yell", "yolk", "yearn", "yet",
                "zebra", "zip", "zipper", "zeal", "zombie"};

#define WORDS_SIZE (long)(sizeof words/sizeof(words[0]))

// Generate the paragraph of words in the typing test
char** createParagraph(int N) {
	char** paragraph = (char**)malloc(N*sizeof(words[0])); // store words list for later comparison
	int limit = 40, used = 0;				// max number of characters per line (visual)
	srand(time(NULL));						// seed for randomness

	for(int i=0; i<N; i++) {
		char *s=words[rand() % WORDS_SIZE];
		char temp[strlen(s)+1];

		char *w;
		sprintf(temp, "%s ", s);		// test ends with space if last word does not match
		w=strdup(temp);

		paragraph[i]=w;
		used += strlen(w);
		if(used<=limit) {
			printf("%s", w);
			if(i+1==N)
				printf("\n");	// new line
		}
		else if (used>limit) {
			printf("%s\n", w);
			used=0;
		}
	}
	return paragraph;
}

// Get user input as an array of characters, convert it to an array of strings and return
// Also updates timestamps for start and end of the typing test
char** getInput(int N, char** paragraph, double* start, double* end) {
	char c;
	char chars[500];	// random large value
	int space_ctr=0, pos=0;
	bool first=true;

	char *last = paragraph[N-1];	// last word
	char last_temp[100];			// stores the user's last word for comparison
	int j=0;						// index for user's last word

	system("stty raw");		// set terminal to raw mode for live char input

	while(space_ctr<N) {
		c=getchar(); putchar(c);
		if(c!='\0' && c!=0x7F && first) {	// start time after first input
			GET_TIME(*start);
			first=false;
		}
		if(c==' ') {
			space_ctr+=1;
			chars[pos] = ' ';
		}
		else if(pos >= 0)
			chars[pos]=c;
		if(c==0x7F) {	// 0x7F = backspace key
			if(pos > 0) {
				printf("\b \b");		// backspace implementation
				pos--;
				if(chars[pos]==' ')
					space_ctr-=1;
				if(j>0)
					j--;
			}
		} else
			pos++;
		
		if(space_ctr+1==N) {		// get last word
			if(c!=' ' && c!=0x7F)
				last_temp[j++]=c;
			if(j==(int)(strlen(last)-1)) {
				last_temp[j]=' ';			// append space (as test ends with space by default)
				last_temp[j+1]='\0';		// end of string
			}
		}
		
		if(strcmp(last, last_temp)==0) {	// check for matching last word
			space_ctr+=1;
			chars[pos++]=' ';
		}
	}
	
	GET_TIME(*end);
	system("stty cooked");		// back to default terminal mode

	// convert to array of strings
	int begin_index=0, end_index, index=0;
	char** to_str = (char**)malloc(N*sizeof(words[0]));
	space_ctr=0;
	for (int x=0; x<pos; x++) {
		char word[500];
		index++;
		if(chars[x]==' ') {
			end_index=index;
			word[0]='\0';
			strncpy(word, chars+begin_index, end_index);
			word[(end_index-begin_index)]='\0';
			to_str[space_ctr]=strdup(word);
			space_ctr++;
			begin_index=end_index;
		}
	}
	return to_str;
}

int main(int argc, char **argv) {
	// should be called by ./typingtest
	if(argc>1) {
		fprintf(stderr, "USAGE: %s\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("\n"); printf("\n"); printf("\n");	// visual

	/* CREATE AND PRINT PARAGRAPH */
	int N = 10;
	char** paragraph = createParagraph(N);

	printf("\n");		// visual

	/* USER INPUT */
	double start=0, end=0;
	char** input = getInput(N, paragraph, &start, &end);

	printf("\n"); printf("\n"); printf("\n");		// visual

	/* COMPARISON AND SCORING */
	int score=0;
	for(int x=0; x<N; x++) {
		if(strcmp(input[x], paragraph[x])==0)
			score++;
		free(input[x]); free(paragraph[x]);
	}
	free(input); free(paragraph);

	float accuracy=((float)score/(float)N)*100;
	float secs = end - start;
	float wpm=(score/secs) * 60;

	printf("\%% correct: %f\n", accuracy);
	printf("WPM: %f\n", wpm);
	printf("\n");

	return EXIT_SUCCESS;
}
