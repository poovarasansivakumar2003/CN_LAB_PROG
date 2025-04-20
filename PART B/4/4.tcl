# Creates a new NS2 simulation instance.
set ns [new Simulator]

# Save simulation logs to 4.tr (text trace) and 4.nam (animation view in NAM tool).
set tf [open 4.tr w]
$ns trace-all $tf
set nf [open 4.nam w]
$ns namtrace-all $nf

# The below code is used to create the nodes.
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

# Connects them via a LAN (Ethernet shared link):
# Bandwidth: 10 Mbps
# Delay: 10 ms
# LL (Link Layer) + DropTail Queue
# MAC/802_3 (Ethernet MAC protocol)
$ns make-lan "$n0 $n1 $n2 $n3" 10mb 10ms LL Queue/DropTail Mac/802_3

# First TCP Flow (n0 → n3)
set tcp0 [new Agent/TCP]
$ns attach-agent $n0 $tcp0
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0
set sink3 [new Agent/TCPSink]
$ns attach-agent $n3 $sink3
$ns connect $tcp0 $sink3

# Second TCP Flow (n2 → n1)
set tcp2 [new Agent/TCP]
$ns attach-agent $n2 $tcp2
set ftp2 [new Application/FTP]
$ftp2 attach-agent $tcp2
set sink1 [new Agent/TCPSink]
$ns attach-agent $n1 $sink1
$ns connect $tcp2 $sink1

# Traces TCP congestion window dynamics:
# file1.tr logs tcp0's cwnd_
# file2.tr logs tcp2's cwnd_
# maxcwnd_ 10 limits tcp0 to a maximum congestion window of 10
set file1 [open file1.tr w]
$tcp0 attach $file1
$tcp0 trace cwnd_
$tcp0 set maxcwnd_ 10

set file2 [open file2.tr w]
$tcp2 attach $file2
$tcp2 trace cwnd_

#define finish procedure
proc finish { } {
global nf tf ns 
$ns flush-trace
exec nam 4.nam &
close $nf
close $tf
exit 0
}

# Event Scheduling
$ns at 0.1 "$ftp0 start"
$ns at 1.5 "$ftp0 stop"
$ns at 2 "$ftp0 start"
$ns at 3 "$ftp0 stop"
$ns at 0.2 "$ftp2 start"
$ns at 2 "$ftp2 stop"
$ns at 2.5 "$ftp2 start"
$ns at 4 "$ftp2 stop"
$ns at 5.0 "finish"
$ns run