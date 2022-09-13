        struct c_token_buffer_stream_chunk
    {
      c_token_buffer Element;
      c_token_buffer_stream_chunk* Next;
    };

        struct c_token_buffer_stream
    {
      memory_arena *Memory;
      c_token_buffer_stream_chunk* FirstChunk;
      c_token_buffer_stream_chunk* LastChunk;
    };

    link_internal void
    Deallocate(c_token_buffer_stream *Stream)
    {
      NotImplemented;
    }

        struct c_token_buffer_iterator
    {
      c_token_buffer_stream* Stream;
      c_token_buffer_stream_chunk* At;
    };

    link_internal c_token_buffer_iterator
    Iterator(c_token_buffer_stream* Stream)
    {
      c_token_buffer_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    link_internal b32
    IsValid(c_token_buffer_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    link_internal void
    Advance(c_token_buffer_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }

    link_internal b32
    IsLastElement(c_token_buffer_iterator* Iter)
    {
      b32 Result = Iter->At->Next == 0;
      return Result;
    }


        link_internal c_token_buffer *
    Push(c_token_buffer_stream* Stream, c_token_buffer Element)
    {
      if (Stream->Memory == 0)
      {
        Stream->Memory = AllocateArena();
      }

      /* (Type.name)_stream_chunk* NextChunk = AllocateProtection((Type.name)_stream_chunk*), Stream->Memory, 1, False) */
      c_token_buffer_stream_chunk* NextChunk = (c_token_buffer_stream_chunk*)PushStruct(Stream->Memory, sizeof(c_token_buffer_stream_chunk), 1, 0);
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

      c_token_buffer *Result = &NextChunk->Element;
      return Result;
    }


