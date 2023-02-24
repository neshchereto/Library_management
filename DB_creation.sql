use library_management;

CREATE TABLE udk_thematic (
    udk_id INT         NOT NULL PRIMARY KEY AUTO_INCREMENT,
    topic  VARCHAR(60) NOT NULL 
);

CREATE TABLE book (
    book_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    title   VARCHAR(80) NOT NULL,
    edition VARCHAR(60),
    udk_id  INT NOT NULL,
    FOREIGN KEY (udk_id) REFERENCES udk_thematic(udk_id)
);

CREATE TABLE author (
    author_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    full_name VARCHAR(25)
);

CREATE TABLE author_book (
    author_id INT NOT NULL,
    book_id   INT NOT NULL,
    FOREIGN KEY (author_id) REFERENCES author (author_id),
    FOREIGN KEY (book_id) REFERENCES book (book_id)
);

CREATE TABLE card (
    inventoryed_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    book_id        INT NOT NULL,
    FOREIGN KEY (book_id) REFERENCES book (book_id)
);

CREATE TABLE reader (
    reader_id    INT          NOT NULL PRIMARY KEY AUTO_INCREMENT,
    full_name    VARCHAR(100) NOT NULL,
    passport     VARCHAR(40)  NOT NULL UNIQUE,
    address      VARCHAR(80),
    mobile       VARCHAR(20),
    birthday     DATE,
    entry_day    DATE,
    password     VARCHAR(20),
    CHECK (birthday  >= '1900-01-01')
);

CREATE TABLE request (
    request_id     INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
    reader_id      INT NOT NULL,
    inventoryed_id INT,
    request_date   DATE NOT NULL
);

CREATE TABLE return_table (
    return_id      INT  NOT NULL PRIMARY KEY AUTO_INCREMENT,
    reader_id      INT  NOT NULL,
    inventoryed_id INT  NOT NULL,
    issue_date     DATE,
    return_date    DATE NOT NULL,
    FOREIGN KEY (inventoryed_id) REFERENCES card (inventoryed_id),
    FOREIGN KEY (reader_id) REFERENCES reader (reader_id)
);

INSERT INTO udk_thematic (udk_id, topic)
VALUES 
(1, 'Philosophy/Psychology'),
(2, 'Religion. Teology'),
(3, 'Social sciencies'),
(4, 'Medicine.'),
(5, 'Maths. Natural Sciences'),
(6, 'Applied Sciences. Technics. Agriculture'),
(7, 'Art. Architecture. Games. Sport'),
(8, 'Language. Linguistics. Fiction.'),
(9, 'Geography. Biography. History');

-- TO DELETE INFORMATION USE
-- DROP TABLE return_table;
-- DROP TABLE request;
-- DROP TABLE reader;
-- DROP TABLE card;
-- DROP TABLE author_book;
-- DROP TABLE author;
-- DROP TABLE book;
-- DROP TABLE udk_thematic;
