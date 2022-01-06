#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

/* WORD LIST */
char *words[] = {
	"what",
	"can",
	"be",
	"because",
	"your",
	"the",
	"a",
	"of",
	"insult",
	"journey",
	"frame",
	"path",
	"flower",
	"mountain",
	"forward",
	"expectation",
	"both",
	"illustrate",
	"ditch",
	"application",
	"request",
	"sent",
	"mail",
	"inches",
	"metric",
	"flame",
	"walk",
	"ancient",
	"test",
	"bucket",
	"art",
	"move",
	"photo",
	"desk",
	"chair",
	"map",
	"bike",
	"price",
	"fault",
	"problem",
	"case",
	"dog",
	"mist",
	"cat",
	"cloud",
	"value",
	"difficult"
};

#define WORDS_SIZE (long)(sizeof words/sizeof(words[0]))

int main(int argc, char **argv) {
	// should be called by ./typingtest
	if(argc>1) {
		fprintf(stderr, "USAGE: %s\n", argv[0]);
		return EXIT_FAILURE;
	}
	printf("\n"); printf("\n"); printf("\n");	// visual

	/* CREATE AND PRINT PARAGRAPH */
	int N = 10;								// number of words in test
	char *paragraph[N*sizeof(words[0])];	// store words list for later comparison
	int limit = 40, used = 0;				// max number of characters per line (visual)
	srand(time(NULL));						// seed for randomness
	for(int i=0; i<N; i++) {
		char *s=words[rand() % WORDS_SIZE];
		char temp[strlen(s)+1];
		char *n;
		sprintf(temp, "%s ", s);		// test ends with space if last word does not match
		n=strdup(temp);

		paragraph[i]=n;
		used += strlen(n);
		if(used<=limit) {
			printf("%s", n);
			if(i+1==N)
				printf("\n");	// new line
		}
		else if (used>limit) {
			printf("%s\n", n);
			used=0;
		}
	}
	printf("\n");		// visual

	/* USER INPUT */
	char c;
	char c_arr[500];	// random large value
	int space_ctr=0, pos=0;
	struct timeval t1, t2;
	long long msecs;
	bool first=true;

	char *last = paragraph[N-1];	// last word
	char last_temp[100];			// stores the user's last word for comparison
	int j=0;						// index for user's last word

	system("stty raw");		// set terminal to raw mode for live char input

	while(space_ctr<N) {
		c=getchar(); putchar(c);
		if(c!='\0' && c!=0x7F && first) {	// start time after first input
			(void)gettimeofday(&t1, NULL);
			first=false;
		}
		if(c==' ') {
			space_ctr+=1;
			c_arr[pos] = ' ';
		}
		else if(pos >= 0)
			c_arr[pos]=c;
		if(c==0x7F) {	// 0x7F = backspace key
			if(pos > 0) {
				printf("\b \b");		// backspace implementation
				pos--;
				if(c_arr[pos]==' ')
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
			c_arr[pos++]=' ';
		}
	}
	
	(void)gettimeofday(&t2, NULL);		// terminate time after last space
	msecs = 1000000*(t2.tv_sec-t1.tv_sec)+(t2.tv_usec-t1.tv_usec);	// milliseconds conversion
	float secs = (float)msecs/1000000;		// seconds conversion
	system("stty cooked");		// back to default terminal mode
	printf("\n"); printf("\n"); printf("\n");		// visual

	// convert to array of strings
	int begin_index=0, end_index, index=0;
	char *to_str[N*sizeof(words[0])];
	space_ctr=0;
	for (int x=0; x<pos; x++) {
		char word[500];
		index++;
		if(c_arr[x]==' ') {
			end_index=index;
			word[0]='\0';
			strncpy(word, c_arr+begin_index, end_index);
			word[(end_index-begin_index)]='\0';
			to_str[space_ctr]=strdup(word);
			space_ctr++;
			begin_index=end_index;
		}
	}

	/* COMPARISON AND SCORING */
	int score=0;
	for(int x=0; x<N; x++) {
		if(strcmp(to_str[x], paragraph[x])==0)
			score++;
		free(to_str[x]); free(paragraph[x]);
	}
	float accuracy=((float)score/(float)N)*100;
	float wpm=(score/secs) * 60;

	printf("\%% correct: %f\n", accuracy);
	printf("WPM: %f\n", wpm);
	printf("\n");

	return EXIT_SUCCESS;
}
