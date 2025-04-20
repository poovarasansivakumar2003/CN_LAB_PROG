# Creates a new NS2 simulation instance.
set ns [ new Simulator ]

# Save simulation logs to 1.tr (text trace) and 1.nam (animation view in NAM tool).
set tf [ open 1.tr w ]
$ns trace-all $tf
set nf [ open 1.nam w ]
$ns namtrace-all $nf

# The below code is used to create the nodes.
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

# This is used to give color and label to the packets.
$ns color 1 "red"
$ns color 2 "blue"
$n0 label "Source/udp0"
$n1 label "Source/udp1"
$n2 label "Router"
$n3 label "Destination/Null"

# Vary the below Bandwidth and see the number of packets dropped. 
# Format: ns duplex-link source dest bandwidth delay queuingPolicy
$ns duplex-link $n0 $n2 10Mb 300ms DropTail
$ns duplex-link $n1 $n2 10Mb 300ms DropTail

# Note: The link from router to destination is slower (1Mb) to create congestion → causes packet drops.
$ns duplex-link $n2 $n3 1Mb 300ms DropTail

# Queue Limits: Defines max packets that can wait in queue before being dropped.
# Lower queue size on the bottleneck link (n2 to n3) means more chances of packet drops.
$ns set queue-limit $n0 $n2 10
$ns set queue-limit $n1 $n2 10
$ns set queue-limit $n2 $n3 5

# The below code is used to attach an UDP agent to n0, UDP #agent to n1 and null agent to n3.
# UDP agents: Represent traffic generators at source nodes.
# CBR: Constant Bit Rate traffic source (sends packets at fixed intervals).
# Null agent: Used at the destination to receive and discard packets (like a "black hole").

set udp0 [new Agent/UDP]
set udp1 [new Agent/UDP]
set cbr0 [new Application/Traffic/CBR]
set cbr1 [new Application/Traffic/CBR]
set null3 [new Agent/Null]

$ns attach-agent $n0 $udp0
$ns attach-agent $n1 $udp1

$cbr0 attach-agent $udp0
$cbr1 attach-agent $udp1

$ns attach-agent $n3 $null3

# The below code sets the udp0 packets to red and udp1 #packets to blue color
$udp0 set class_ 1
$udp1 set class_ 2

# The below code is used to connect the agents.
$ns connect $udp0 $null3
$ns connect $udp1 $null3

# The below code is used to set the packet size to 500
# packetSize_: Each packet is 500Mb in size (likely you meant 500 bytes, this is a bit too large).
# interval_: Time between packets. 0.005 seconds = 200 packets/second → high traffic.
$cbr1 set packetSize_ 500Mb
$cbr1 set interval_ 0.005 

# Simulation Control: Finish Procedure
proc finish { } { 
global ns nf tf
$ns flush-trace
exec nam 1.nam &
close $tf
close $nf
exit 0
}

# Schedule Events and Run
# Start both traffic flows at 0.1 seconds.
# End simulation at 10.0 seconds.
$ns at 0.1 "$cbr0 start"
$ns at 0.1 "$cbr1 start"
$ns at 10.0 "finish"
$ns run