CREATE TABLE todos (
          id SERIAL PRIMARY KEY NOT NULL,
          title VARCHAR NOT NULL,
          completed BOOLEAN NOT NULL
        )