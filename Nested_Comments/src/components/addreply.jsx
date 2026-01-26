export const addReply = (comments, parentId, newReply) => {
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
