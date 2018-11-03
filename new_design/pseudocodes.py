

# Structures used
pl_ACK # list of counters for sending over perfect link
# initialized to all zeros

urb_ACK # N x N Matrix for URB broadcast takes care of pending and ack[m] rows are senders, columns are receivers
# initialized to all zeros

delivered # List of counters of delivered messages
# initialized to all zeros
ACK_NUMBER 



# FAIR LOSS
def send_fl(sequence_nr,original_sender,pid,destination):
    # Just sends the msg
def deliver_fl(sequence_nr,original_sender,sender):
    send_fl(ACK_NUMBER,original_sender,pid,sender)
    
        deliver_pl(sequence_nr,original_sender,sender)




def send_pl(sequence_nr,original_sender,pid,destination):
    send_fl(sequence_nr,original_sender,pid,destination)
    wait(1)
    while pl_ACK[destination] < sequence_nr:
        send_fl(sequence_nr,original_sender,pid,destination)
        wait(1)

def deliver_pl(sequence_nr,original_sender,sender):
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