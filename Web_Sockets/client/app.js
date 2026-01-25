const socket = io("http://localhost:3001");

const form = document.querySelector("form");
const p = document.querySelector("p");
const input = document.querySelector("input");
const ul = document.querySelector("ul");

form.addEventListener("submit", (e) => {
    e.preventDefault();

    if (input.value.trim()) {
        socket.emit("message", input.value);
        input.value = "";
    }
});

// Typing indicator (debounced)
let typingTimeout;
input.addEventListener("input", () => {
    socket.emit("activity", socket.id);

    clearTimeout(typingTimeout);
    typingTimeout = setTimeout(() => {
        p.textContent = "";
    }, 1000);
});

socket.on("message", (data) => {
    const li = document.createElement("li"); // ✅ NEW li
    li.textContent = data;
    ul.appendChild(li);
     ul.scrollTop = ul.scrollHeight;
    p.textContent = "";
});


let timer;
socket.on("activity", (name) => {
    p.textContent = `${name.substring(0, 5)} is typing...`;
    clearTimeout(timer)
    timer=setTimeout(()=>{
        p.textContent=""
    },2000)
});
