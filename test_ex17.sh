#/bin/bash

echo Compiling.....
if make ex17; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo Creating default database file
if ./ex17 db.dat c; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo Inputting test data
if ./ex17 db.dat s 1 "allen eubank" "allen@eubank.com"; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo Retrieving test data
if ./ex17 db.dat g 1; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo Input more data
if ./ex17 db.dat s 2 "Tom Major" "tom@major.com" && \
      ./ex17 db.dat s 3 "Bob Hope" "bob@hope.com"; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo List database
if ./ex17 db.dat l; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo Delete data
if ./ex17 db.dat d 1-2; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo List database
if ./ex17 db.dat l; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

echo Find data
if ./ex17 db.dat f "bob"; then
  echo Success
else
  echo Fail 1>&2
  exit 1
fi

exit 0
