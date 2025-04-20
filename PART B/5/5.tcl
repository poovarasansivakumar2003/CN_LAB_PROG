# Creates a new NS2 simulation instance.
set ns [new Simulator]

# Save simulation logs to 5.tr (text trace) and 5.nam (animation view in NAM tool).
set tf [open 5.tr w]
$ns trace-all $tf
set nf [open 5.nam w]
$ns namtrace-all $nf

# The below code is used to create the nodes.
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]

# This is used to give color and label to the packets.
$ns color 1 "red"
$n1 label "Source/UDP"
$n3 label "Error Node"
$n5 label "Destination"

# Connects them via a two Lans (Lan1 and Lan2) (Ethernet shared link):
# Bandwidth: 10 Mbps
# Delay: 10 ms
# LL (Link Layer) + DropTail Queue
# MAC/802_3 (Ethernet MAC protocol)
$ns make-lan "$n0 $n1 $n2 $n3" 10Mb 10ms LL Queue/DropTail Mac/802_3
$ns make-lan "$n4 $n5 $n6" 10Mb 10ms LL Queue/DropTail Mac/802_3

#The below code is used to connect node n3 of lan1 and n6 of #lan2.
$ns duplex-link $n3 $n6 100Mb 10ms DropTail

set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1

set cbr1 [ new Application/Traffic/CBR]
$cbr1 attach-agent $udp1

set null5 [new Agent/Null]
$ns attach-agent $n5 $null5

$ns connect $udp1 $null5

$cbr1 set packetSize_ 1000
$cbr1 set interval_ 0.0001 ;# This is the data rate. Change this to increase the rate.

$udp1 set class_ 1

# The below code is used to add an error model between the #nodes n3 and n6.
set err [new ErrorModel]
$ns lossmodel $err $n3 $n6
$err set rate_ 0.2 ;# This is the error rate. Change this rate to add errors between n3 and n6.

# define finish procedure
proc finish { } {
global nf ns tf
exec nam 5.nam &
close $nf
close $tf
exit 0
}

# Schedule Events and Run
$ns at 0.1 "$cbr1 start"
$ns at 5.0 "finish"
$ns run
