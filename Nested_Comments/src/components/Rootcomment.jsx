import { useState } from "react";
import Comment from "./Comment";

const Rootcomment = ({ comments, setComments }) => {
  const [text, setText] = useState("");

  const addRootComment = (e) => {
    e.preventDefault();

    if (!text.trim()) return;

    const newComment = {
      id: Date.now(),
      name: `user_${Math.floor(Math.random() * 1000)}`,
      text,
      replies: []
    };

    setComments([...comments, newComment]);
    setText("");
  };

  return (
    <>
      {/* ✅ MAIN COMMENT BOX */}
      <form onSubmit={addRootComment}>
        <textarea
          placeholder="Add a comment..."
          rows={4}
          cols={50}
          value={text}
          onChange={(e) => setText(e.target.value)}
        />
        <br />
        <button type="submit">Comment</button>
      </form>

      <hr />

      {/* Render all root comments */}
      {comments.map((comment) => (
        <Comment
          key={comment.id}
          comment={comment}
          comments={comments}
          setComments={setComments}
        />
      ))}
    </>
  );
};

export default Rootcomment;
