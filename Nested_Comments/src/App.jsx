import { useState } from "react";
import Rootcomment from "./components/Rootcomment";
import "./App.css";

function App() {
  const initialComments = [
    {
      id: 1,
      name: "skmda",
      text: "This explanation of Dijkstra’s algorithm is really clear 👍",
      replies: []
    }
  ];

  const [comments, setComments] = useState(initialComments);

  return (
    <Rootcomment comments={comments} setComments={setComments} />
  );
}

export default App;
