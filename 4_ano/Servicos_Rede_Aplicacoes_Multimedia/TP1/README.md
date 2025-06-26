# Multimedia Transport Protocol

This project implements a UDP-based communication system that transmits images from a server to a client through an intermediate forwarder. The system simulates packet loss, transmission pauses, and automatic resynchronization, aiming to test fault tolerance and synchronization mechanisms over unstable networks.

---

### Key Parameters

* `F`: Frequency factor controlling the interval between image transmissions (`10^(-F)` seconds between frames).
* `A`: Display delay per frame (milliseconds).
* `N`: Number of frames after which the forwarder pauses.
* `P`: Pause duration in seconds.
* `M`: Interval (in seconds) between simulated packet losses by the forwarder.

---

## How It Works

### Server (`server.py`)

* Dynamically generates an **image clock** showing the current time (e.g., `12:34:56:78`).
* Builds an image by composing PNG digits located in the `digitos/` directory.
* Supports fractional seconds based on the value of `F` (e.g., `F=2` → shows centiseconds).
* Encodes the image and sends it via UDP to the forwarder, in the form of a **Protocol Data Unit (PDU)**.

#### PDU Format

| Field | Size     | Description                                         |
| ----- | -------- | --------------------------------------------------- |
| `F`   | 1 byte   | Precision level (number of digits for milliseconds) |
| `A`   | 2 bytes  | Display delay in milliseconds                       |
| Image | variable | Binary-encoded PNG image of the clock               |

---

### Forwarder (`reencaminhador.py`)

* Listens for UDP packets (PDUs) from the server.
* Buffers incoming frames using a queue.
* Simulates **packet loss** by discarding one PDU every `M` seconds.
* Simulates **pauses** by halting transmission for `P` seconds after every `N` frames.
* Forwards valid PDUs from the buffer to the client.

---

### Client (`client.py`)

* Receives PDUs and extracts:

  * Control fields: `F`, `A`
  * Image payload (PNG)
* Maintains a **timed buffer** of frames.
* Displays frames after their delay `A` has passed.
* If the buffer exceeds a threshold (computed from `F` and `A`), it **resynchronizes**:

  * Waits for `A` ms
  * Clears the buffer
  * Resumes receiving

---

