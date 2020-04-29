## Design
  原理很簡單，分配兩個cores，一個run processes，另外一個scheduling。設置一個process queue決定processes的順序，每當process開始或交換的時候，就會enqueue 當前的process。之後，當core 0(running processes)閒置的時候，就會dequeue。Operations 分三種，分別是dequeue, enqueue, low execution time priority enqueue。
 
  FIFO: 開始fork的時候enqueue， 執行的時候dequeue。
 
  PSJF: 當新的process開始的時候，會yield當前的process，然後執行兩個low execution time priority enqueue，分別enqueue開始和當前的process。

  RR: 基本上，與FIFO類似，但會讓running process跑一個quantum time之後yield，然後enqueue再dequeue直到跑完。
 
  SJF: 當新的process開始的時候，執行low execution time priority enqueue。與PSJF只差在SJF並沒有yield當前在跑的process。
 
## Kernel Version
    Ubuntu 4.14.176
   
## Practice versus theory
  在TIME_MEASUREMENT.txt測試的時候，start time 和end time差距有時候會突然暴漲。總體來說，雖然閒置與跑process都是500個unit time，但是實際閒置core的時間會比跑process的還要少。照理來說，core 1(scheduling core)會延遲因為作比較多的operation，造成相反的結果，一種可能是syscall需要花很多時間，延遲了process 結束的時間。
