

# Structures used
pl_ACK # list of counters for sending over perfect link
# initialized to all zeros

urb_ACK # N x N Matrix for URB broadcast takes care of pending and ack[m] rows are senders, columns are receivers
# initialized to all zeros

delivered # List of counters of delivered messages
# initialized to all zeros
ACK_NUMBER 



# FAIR LOSS
def send_fl(int fd, struct sockaddr_in* address, msg_t* msg):
    # Just sends the msg
def deliver_fl(receiver_info_t* data, struct sockaddr_in* sender, msg_t* msg):
        deliver_pl(struct sockaddr_in* sender, msg_t* msg)



# PERFECT LINK
# we want 1 sender thread to send to many process, so that it doesnt block on 1 process, we implement a threshold
# if the pl_send loops more than the threshold we move this task (of sending this message to that process) at the end of the
# task queue and send the next message from the task list
def send_pl(sequence_nr,original_sender,pid,destination):
        loop_i = 0
        while pl_ACK[destination] < sequence_nr:
                if(loop_i >= THRESHOLD):
                        return BLOCKED_VALUE
                send_fl(int fd, struct sockaddr_in* address, msg_t* msg)
                loop_i += 1
                wait(lambda)
        return SUCCESS_VALUE


def deliver_pl(sequence_nr,original_sender,sender):
        if(sequence_nr not in delivered)
                delivered.add(sequence_nr)
                deliver_beb(sequence_nr,original_sender,sender)




def beb(sequence_nr,original_sender,pid,destinations):
        for i in destinations:
                send_pl(sequence_nr,original_sender,pid,i)

def urb(sequence_nr,pid,destinations):
    urb_ACK[pid][pid] += 1
    beb(sequence_nr,pid,pid,destinations)


def fifob(num_msgs,pid,destinations):
    for i in range(1,num_msgs + 1):
        urb(i,pid,destinations)






def candeliver(sequence_nr,original_sender):
        if count(urb_ACK[original_sender],x >= sequence_nr) > N/2:
                return True
        else return False

def deliver_beb(sequence_nr,original_sender,sender):
    if urb_ACK[original_sender][sender] == sequence_nr -1:
            urb_ACK[original_sender][sender] += 1
    if candeliver :
            
    

def deliver_urb(sequence_nr,original_sender,sender)

def deliver_fifo(sequence_nr,original_sender,sender)

def deliver_final(sequence_nr,original_sender,sender)