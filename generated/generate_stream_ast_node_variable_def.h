        struct ast_node_variable_def_stream_chunk
    {
      ast_node_variable_def Element;
      ast_node_variable_def_stream_chunk* Next;
    };

        struct ast_node_variable_def_stream
    {
      ast_node_variable_def_stream_chunk* FirstChunk;
      ast_node_variable_def_stream_chunk* LastChunk;
    };


        struct ast_node_variable_def_iterator
    {
      ast_node_variable_def_stream* Stream;
      ast_node_variable_def_stream_chunk* At;
    };

    link_internal ast_node_variable_def_iterator
    Iterator(ast_node_variable_def_stream* Stream)
    {
      ast_node_variable_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    link_internal b32
    IsValid(ast_node_variable_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    link_internal void
    Advance(ast_node_variable_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }

    link_internal b32
    IsLastElement(ast_node_variable_def_iterator* Iter)
    {
      b32 Result = Iter->At->Next == 0;
      return Result;
    }


        link_internal ast_node_variable_def *
    Push(ast_node_variable_def_stream* Stream, ast_node_variable_def Element, memory_arena* Memory)
    {
      ast_node_variable_def_stream_chunk* NextChunk = (ast_node_variable_def_stream_chunk*)PushStruct(Memory, sizeof(ast_node_variable_def_stream_chunk), 1, 0);
      NextChunk->Element = Element;

      if (!Stream->FirstChunk)
      {
        Assert(!Stream->LastChunk);
        Stream->FirstChunk = NextChunk;
        Stream->LastChunk = NextChunk;
      }
      else
      {
        Stream->LastChunk->Next = NextChunk;
        Stream->LastChunk = NextChunk;
      }

      Assert(NextChunk->Next == 0);
      Assert(Stream->LastChunk->Next == 0);

      ast_node_variable_def *Result = &NextChunk->Element;
      return Result;
    }

    link_internal void
    ConcatStreams( ast_node_variable_def_stream *S1, ast_node_variable_def_stream *S2)
    {
      if (S1->LastChunk)
      {
        Assert(S1->FirstChunk);

        if (S2->FirstChunk)
        {
          Assert(S2->LastChunk);
          S1->LastChunk->Next = S2->FirstChunk;
          S1->LastChunk = S2->LastChunk;
        }
        else
        {
          Assert(!S2->LastChunk);
        }
      }
      else
      {
        Assert(!S1->FirstChunk);
        Assert(!S1->LastChunk);

        if(S2->FirstChunk)
        {
          Assert(S2->LastChunk);
        }
        else
        {
          Assert(!S2->LastChunk);
        }

        *S1 = *S2;
      }
    }


