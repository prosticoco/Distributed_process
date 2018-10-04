



#define MESSAGE_LENGTH 256

typedef struct{
		size_t msg_type;
		int srcId;
		char mtext[MESSAGE_LENGTH];
}message;
