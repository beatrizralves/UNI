# Data Plane

Implementing of a firewall using the P4 programming language in a network virtualization environment. The firewall was designed to control traffic between two hosts, allowing only specific TCP traffic while blocking all other traffic.

- Implemented using P4 to program packet processing.

- A table named firewall was created with four match keys: source IP, destination IP, TCP source port, and TCP destination port.

- Only TCP traffic from any port of h1 to port 5555 of h2, and from port 5555 of h2 to any port of h1, is allowed.

- The default action is drop, discarding all packets not explicitly permitted.
