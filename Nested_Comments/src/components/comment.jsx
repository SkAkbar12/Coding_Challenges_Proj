import { useState } from "react";

const Comment = ({ comment, comments, setComments, level = 0 }) => {
  const [show, setShow] = useState(true);
  const [replyBox, setReplyBox] = useState(false);
  const [editBox, setEditBox] = useState(false);
  const [text, setText] = useState(comment.text);


  const deleteReply = (comments, replyId) => {
  return comments
    .filter(comment => comment.id !== replyId)
    .map(comment => ({
      ...comment,
      replies: deleteReply(comment.replies, replyId)
    }));
};


const editReply = (comments, replyId, newText) => {
  return comments.map(comment => {
    if (comment.id === replyId) {
      return { ...comment, text: newText };
    }
    return {
      ...comment,
      replies: editReply(comment.replies, replyId, newText)
    };
  });
};


const addReply = (comments, parentId, newReply) => {
  return comments.map(comment => {
    if (comment.id === parentId) {
      return {
        ...comment,
        replies: [...comment.replies, newReply]
      };
    }
    return {
      ...comment,
      replies: addReply(comment.replies, parentId, newReply)
    };
  });
};

  // ADD REPLY
  const submitReply = (e) => {
    e.preventDefault();

    const newReply = {
      id: Date.now(),
      name: "user_" + Math.floor(Math.random() * 1000),
      text,
      replies: []
    };

    setComments(addReply(comments, comment.id, newReply));
    setText("");
    setReplyBox(false);
    setShow(true);
  };

  // EDIT
  const submitEdit = () => {
    setComments(editReply(comments, comment.id, text));
    setEditBox(false);
  };

  // DELETE
  const handleDelete = () => {
    setComments(deleteReply(comments, comment.id));
  };

  return (
    <div
      style={{
        marginLeft: level * 25,
        border: "1px solid gray",
        padding: 10
      }}
    >
      <p><strong>{comment.name}</strong></p>

      {editBox ? (
        <>
          <textarea value={text} onChange={(e) => setText(e.target.value)} />
          <br />
          <button onClick={submitEdit}>Save</button>
          <button onClick={() => setEditBox(false)}>Cancel</button>
        </>
      ) : (
        <p>{comment.text}</p>
      )}

      <button onClick={() => setReplyBox(!replyBox)}>Reply</button>
      <button onClick={() => setEditBox(!editBox)}>Edit</button>
      <button onClick={handleDelete}>Delete</button>
      <button onClick={() => setShow(!show)}>
        {show ? "Hide Replies" : "Show Replies"}
      </button>

      {replyBox && (
        <form onSubmit={submitReply}>
          <textarea
            rows={3}
            value={text}
            onChange={(e) => setText(e.target.value)}
          />
          <br />
          <button type="submit">Submit</button>
        </form>
      )}

      {show &&
        comment.replies.map((reply) => (
          <Comment
            key={reply.id}
            comment={reply}
            comments={comments}
            setComments={setComments}
            level={level + 1}
          />
        ))}
    </div>
  );
};

export default Comment;
