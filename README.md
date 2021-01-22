# Linux-system-programming
Socket, log-in and reservation in C
*If you want to use this program, you should run the "MAkefile" which is in each directory

------------------------------------------------------------------
0. An input example(for client)
  ex1.
  NAME(ID)
  0000(PW) //Correct password
  3(product number)
  1(order quantity)

  ex2.
  NAME(ID)
  0000(PW) //Worng password
  y(retry)
  9999(PW) //Correct password
  3(product number)
  1(order quantity)
------------------------------------------------------------------
1. Log-in
  -Enter ID
  -If this ID exists, the client would enter the password and be checked if it is correct.
   If it's not correct, the server would ask the client to enter the password again.
   When the client says "y", the user could retry.
  -If this ID is not existed, 
   the server would make new directory and take the password and save.
  -The password should be a 4-letter word.
   If it is more than 4 letters, the server would save only the first 4 letters.
   
1. 로그인
  -Id입력
  -기존에 있는 id이면 pw를 입력받고 일치하는지 확인,
   불일치면 다시 입력하는지 묻고 재시도
  -기존에 없는 새 id이면 새로운 dir생성, pw를 입력받고 저장
  -pw는 4글자, 그 이상받아도 앞 4글자만 저장됨
------------------------------------------------------------------
2. Show the List of Products and Price.
  - When the client logs in successfully, the server gives the list of products and price to the client.
  
2. 상품 출력
  - 로그인을 성공하면 서버에서 상품과 가격을 받아 출력함
------------------------------------------------------------------
3. Pick One.
  - The client should enter the number of a product that the customer wants.

3. 상품 고르기
  - 원하는 상품의 번호를 입력
------------------------------------------------------------------
4. The Quantity
  - Enter the quantity how many the customer wants it.

4. 상품 수량 
  - 원하는 수량을 입력
------------------------------------------------------------------
5. Complete the order and Save
  - Save the details of the order such as product's name, quantity and total bill.

5. 주문완료 & 저장
  - 주문 완료된 상세정보(주문한 상품, 갯수, 가격)을 서버에 저장 
  - 클라이언트에게 출력
------------------------------------------------------------------
6. The 5min timeout works when there is no interactive activies to server.

6. 5분이상 입력없으면 강제종료
------------------------------------------------------------------

