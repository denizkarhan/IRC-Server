## IRC (Internet Relay Chat) Nedir?
Kullanıcıların anlık olarak metin tabanlı mesajlar gönderip alabildiği bir anlık mesajlaşma protokolüdür.
IRC sunucuları, kullanıcıların IRC istemcileri aracılığıyla bağlanmasına ve birbirleriyle sohbet etmesine olanak tanır.
Aynı zamanda operatörler (IRC operatörleri veya IRC op'ları) adı verilen kullanıcılara, sunucunun yönetimini ve denetimini sağlama yetkisi verir.
Her bir IRC ağı, kendi IRC sunucularından oluşur ve kendi yöneticileri tarafından denetlenir.
Bu sunucular dünya genelinde birçok farklı IRC ağI üzerinde faaliyet gösterebilirler.
İnternetin erken dönemlerinde popülerleşmiş ve hâlâ birçok sunucusu ve kullanıcısı bulunmaktadır.
Açık kaynaklı bir protokol olduğu için, kullanıcılar kendi IRC sunucularını veya istemci yazılımlarını oluşturabilir ve kullanabilirler.
IRC, özellikle teknik topluluklar, açık kaynak yazılım projeleri ve diğer ilgi grupları arasında iletişim için hala kullanılan bir protokoldür.

<p align="center">
  <img src="https://user-images.githubusercontent.com/81527587/232305724-6ba1fbda-caa8-4a64-bee7-20b8685ff3a6.png" width="400" height="400" alt="IRC Network">
</p>

### IRC bağlantısı nasıl uygulanır?
IRC protokolü, TCP/IP tabanlı bir protokol olduğu için socket programlama kullanılarak sunuculara bağlanma, veri gönderme ve veri alma işlemleri gerçekleştirilir.
Socket programlama, ağ iletişimini gerçekleştirmek için kullanılan bir programlama yöntemidir.
Bir soket, bir ağ üzerindeki iki nokta arasındaki iletişimi temsil eder.
IRC sunucuları da birer ağ noktasıdır ve kullanıcıların bu noktalara bağlanarak iletişim kurmalarına izin verir.
Bir IRC istemcisi, IRC sunucusuna bir soket oluşturarak bağlanır, sunucuyla veri alışverişi yapar ve kullanıcıların IRC kanallarına veya özel mesajlaşma yoluyla iletişim kurmalarına olanak tanır.
Socket programlama, IRC sunucularıyla etkileşimde bulunmak için düşük seviyeli bir yaklaşımdır çünkü IRC protokolü düşük seviyeli bir protokoldür ve doğrudan TCP/IP tabanlı ağ bağlantılarını kullanır.
Socketler IRC sunucularında daha esnek ve özelleştirilmiş bir etkileşim sağlar, özellikle daha karmaşık işlemler,
özel komutlar veya IRC sunucularının yönetimine ilişkin operatör yetkileri gibi özellikler gerektiğinde kullanışlıdır.

<p align="center">
  <img src="https://user-images.githubusercontent.com/81527587/232306875-82dfffe7-8508-48ab-84b3-2f6326ebac9a.png" width="420" height="580" alt="Clients">
</p>

### Server ve Client bilgisi
IRC ağlarında, kullanıcılar çeşitli IRC istemcileri (client) ile sunuculara bağlanarak, kanallar veya özel mesajlar aracılığıyla birbirleriyle iletişim kurabilirler.

IRC istemcileri (client), kullanıcıların IRC ağlarına bağlanmalarını ve IRC sunucuları ile iletişim kurmalarını sağlar.
Kullanıcılar, IRC istemcileri aracılığıyla sunuculara bağlanarak, kullanıcı adı (nick) ve şifre gibi kimlik doğrulama bilgilerini kullanarak IRC ağına giriş yaparlar.
Ardından, kullanıcılar, IRC komutlarını kullanarak kanallara veya diğer kullanıcılara mesaj gönderebilir, kanallara katılabilir, kanallardan ayrılabilir,
kullanıcıları engelleyebilir ve diğer birçok IRC işlemini gerçekleştirebilirler.

IRC sunucuları (server), kullanıcıların IRC ağına bağlanmasına izin veren ve kullanıcıların mesajlarını,
kanal bilgilerini ve diğer verileri aktaran sunucu yazılımlarıdır.
IRC sunucuları, kullanıcıların bağlantılarını kabul eder ve kullanıcıların kimlik doğrulama bilgilerini doğrular.
Sunucular, kullanıcıların mesajlarını, komutlarını ve diğer IRC verilerini kanallara veya kullanıcılara iletir.
Genellikle dağıtık bir yapıda çalışır ve birden fazla sunucu arasında veri senkronizasyonunu sağlamak için protokoller kullanır.
IRC istemci ve sunucuları arasındaki iletişim, genellikle IRC protokolü olarak bilinen standart bir metin tabanlı protokol üzerinden gerçekleştirilir.
IRC protokolü, istemcilerin ve sunucuların birbirleriyle iletişim kurmak için kullanacakları komutları, mesaj biçimlerini ve protokol kurallarını tanımlar.
Örneğin, kullanıcılar "/join" komutunu kullanarak bir kanala katılabilir,
"/msg" komutunu kullanarak bir kullanıcıya özel bir mesaj gönderebilir veya "/quit" komutunu kullanarak IRC ağından çıkabilirler.
IRC istemci ve sunucuları, kullanıcıların gerçek zamanlı iletişim kurmasına olanak tanır ve çeşitli IRC komutları ve mesaj biçimleri kullanılarak zengin bir iletişim deneyimi sunar.

<p align="center">
  <img src="https://user-images.githubusercontent.com/81527587/232305996-f93be640-5ab6-49db-b44c-630d40ea106a.png" width="500" height="450" alt="Clients">
</p>

<details>
  <summary>
    <h3 style="display:inline">Popüler IRC Clientler</h2>
  </summary>

* HexChat: Windows, macOS, Linux gibi farklı platformlarda çalışabilen ücretsiz ve açık kaynak kodlu bir IRC istemcisidir. Kullanıcı dostu bir arayüze sahiptir ve pek çok özelliği destekler.

* irssi: Linux ve macOS gibi Unix tabanlı sistemlerde kullanılabilen ücretsiz ve açık kaynak kodlu bir IRC istemcisidir. Komut satırı tabanlı bir arayüze sahiptir ve güçlü bir betikleme yetenekleri sunar.

* WeeChat: Linux, macOS, BSD ve Windows gibi farklı platformlarda çalışabilen ücretsiz ve açık kaynak kodlu bir IRC istemcisidir. Komut satırı tabanlı bir arayüze sahiptir, ancak kullanıcı dostu bir eklenti sistemi sayesinde özelleştirilebilir.

* mIRC: Windows tabanlı bir IRC istemcisidir ve ücretlidir. Popüler ve kullanıcı dostu bir arayüze sahiptir, birçok özelliği destekler ve geniş bir eklenti koleksiyonu bulunur.

* Quassel IRC: Windows, macOS, Linux gibi farklı platformlarda çalışabilen açık kaynak kodlu bir IRC istemcisidir. İstemci ve sunucu olmak üzere iki bileşenden oluşur, böylece kullanıcıların istemciler arasında oturumlarını senkronize etmelerine olanak tanır.

</details>


## Kullanılan socket fonksiyonları

* ```int socket(int domain, int type, int protocol);```

  - int domain: Soketin etki alanı - Oluşturulacak soketin etki alanını belirtir.
  Örneğin, IPv4 adreslerini kullanmak için AF_INET veya IPv6 adreslerini kullanmak için AF_INET6 gibi değerler kullanılabilir.
  - int type: Soketin tipi - Oluşturulacak soketin tipini belirtir. Örneğin, TCP (Stream) soketler için SOCK_STREAM veya UDP (Datagram) soketler için SOCK_DGRAM gibi değerler kullanılabilir.
  - int protocol: Soketin protokolü - Oluşturulacak soketin kullanacağı protokolü belirtir. Genellikle 0 değeri kullanılarak, domain ve tip parametrelerine bağlı olarak uygun bir protokol otomatik olarak seçilir.
  socket() fonksiyonu, başarılı bir şekilde tamamlandığında yeni bir soketin tanımlayıcısını (socket descriptor) döndürür. Hata durumunda -1 döndürerek bir hata kodu ile fonksiyonun tamamlandığını belirtir.

* ```int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);```

  - sockfd: Soketin tanımlayıcısı (socket descriptor) - Değiştirilecek soketin tanımlayıcısıdır.
  - int level: Seçeneğin seviyesi - Ayarlanmak istenen seçeneğin seviyesini belirtir.
    Genellikle SOL_SOCKET seviyesi kullanılır, soket seviyesindeki seçenekleri temsil eder.
    Diğer seviyeler, protokol seviyelerindeki (örneğin, TCP veya UDP) seçenekleri değiştirmek için kullanılabilir.
  - int optname: Seçeneğin adı - Ayarlanmak istenen seçeneğin adını belirtir.
    Örnek olarak, SO_REUSEADDR gibi bir soket seçeneği adı kullanılabilir.
  - const void *optval: Seçeneğin değeri - Ayarlanmak istenen seçeneğin değerini belirtir.
    Bu parametre için bir işaretçi kullanılır.
  - socklen_t optlen: Seçeneğin değerinin boyutu - optval parametresinin işaret ettiği bellek alanının boyutunu belirtir.

* ```int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);```

  - sockfd: Bağlanacak soketin dosya tanımlayıcısı (file descriptor).
  - addr: Bağlanacak adresin yapısını temsil eden bir struct sockaddr işaretçisi. IPv4 adresleri için struct sockaddr_in yapısı, IPv6 adresleri için struct sockaddr_in6 yapısı kullanılır.
  - addrlen: addr yapıının boyutunu temsil eden bir socklen_t değeri.
  
    ```markdown
    bind() fonksiyonu, bir soketi belirtilen IP adresi ve port numarasına bağlar.
    IP adresi, genellikle struct sockaddr_in veya struct sockaddr_in6 yapılarındaki sin_addr veya sin6_addr alanına atanırken,
    port numarası da sin_port veya sin6_port alanına atanır.

* ```int listen(int sockfd, int backlog);```

  - sockfd: Bağlantıları dinlenecek soketin dosya tanımlayıcısı (file descriptor).
  - backlog: Kuyruğa alınacak en fazla bağlantı sayısını temsil eden bir tam sayı değeri.
  
    ```markdown
    listen() fonksiyonu, belirtilen soketi pasif modda dinlemeye başlar.
    backlog parametresi, dinlenecek bağlantıların kuyruğa alınabileceği maksimum sayıyı belirler.
    Eğer bu kuyruk dolarsa, yeni bağlantılar reddedilir.

* ```int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);```

  - sockfd: Bağlantı taleplerini kabul edecek olan soketin dosya tanımlayıcısı (file descriptor).
  - addr: Yeni bağlantının adres bilgisini içeren bir struct sockaddr işaretçisi. İstemci bağlantısı bilgileri bu yapıya yazılır.
  - addrlen: addr yapısının boyutunu temsil eden bir socklen_t işaretçisi.
    Başlangıçta, bu değişkenin değeri addr yapısının boyutunu içermelidir.
    accept() fonksiyonu tarafından gerçekleştirilen bağlantı ile ilgili adres bilgisi bu değişkene yazılır.
  
    ```markdown
    accept() fonksiyonu, listen() fonksiyonu ile belirtilen soketi dinler ve bir istemci bağlantısı kabul eder.
    Yeni bir soket oluşturarak istemci ile iletişim kurmak için kullanılır.
    addr ve addrlen parametreleri, kabul edilen istemci bağlantısının adres bilgilerini içerir.

* ```ssize_t recv(int sockfd, void *buf, size_t len, int flags);```

  - sockfd: Veri alınacak soketin dosya tanımlayıcısı (file descriptor).
  - buf: Alınan verinin okunacağı bellek tamponunun işaretçisi.
  - len: Okunmak istenen verinin maksimum uzunluğunu temsil eden bir tam sayı değeri.
  - flags: İsteğe bağlı bayraklar (flags) parametresi.
    Bu parametre, veri alımı sırasında farklı davranışlar belirlemek için kullanılabilir. Genellikle 0 olarak belirtilir.
  
    ```markdown
    recv() fonksiyonu, belirtilen soketten veri okuyarak buf işaretçisine kopyalar.
    len parametresi, okunacak maksimum veri uzunluğunu belirler.
    Fonksiyon, gerçekten okunan veri uzunluğunu döndürür. Okunan veri yoksa 0 döndürür.

* ```int poll(struct pollfd *fds, nfds_t nfds, int timeout);```

  - fds: struct pollfd türündeki bir dizi, her bir soketin durumunu ve dinlenecek olayları temsil eden yapıları içerir.
  - nfds: fds dizisinin eleman sayısı.
  - timeout: Fonksiyonun bloklanma süresini belirten bir zaman aşımı değeri.
    timeout değeri negatif bir sayı ise poll() fonksiyonu bloklu olarak çalışır ve olay meydana gelene kadar bekler.
    timeout değeri 0 ise poll() fonksiyonu hemen döner ve olayların durumunu kontrol eder.

      struct pollfd yapısı, poll() fonksiyonunda kullanılan her bir soketin durumunu ve dinlenecek olayları temsil eder. Bu yapı aşağıdaki gibidir:
      ```C++
      struct pollfd {
          int fd;
          short events;
          short revents;
      };

    - fd: Soketin dosya tanımlayıcısı (file descriptor).
    - events: Dinlenen olayları belirten flags.
        Hangi olayların dinleneceği bu alanda belirtilir. Örneğin POLLIN okuma olayını, POLLOUT yazma olayını  temsil eder.
    - revents: Geri dönen olayları (returned events) belirten bayraklar (flags).
        Bu alan, poll() fonksiyonu tarafından geri döndürülen olayları temsil eder.
        Hangi olayların  meydana geldiği bu alanda belirtilir.

    poll() fonksiyonu, fds dizisinde belirtilen soketleri dinler ve olayların meydana gelip gelmediğini kontrol eder.
    events alanında belirtilen olayların meydana gelmesi durumunda, revents alanında ilgili bayraklar (flags) ayarlanır.

