const express = require("express");
const { Server } = require("socket.io");

const app = express();

const server = app.listen(3001, () => {
    console.log("Server running on port 3001");
});

const io = new Server(server, {
    cors: {
        origin: "*"
    }
});

io.on("connection", (socket) => {
    console.log(`User ${socket.id} connected`);

    socket.emit("message", "Welcome to online chat app");

    socket.broadcast.emit(
        "message",
        `User ${socket.id.substring(0, 5)} connected`
    );

    socket.on("activity", (name) => {
        socket.broadcast.emit("activity", name);
    });

    
    socket.on("disconnect", () => {
        socket.broadcast.emit(
            "message",
            `User ${socket.id.substring(0, 5)} disconnected`
        );
    });

    socket.on("message", (data) => {
        io.emit("message", `${socket.id.substring(0, 5)} : ${data}`);
    });
});
