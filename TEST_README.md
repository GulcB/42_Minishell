# Minishell Test Suite

Bu repository'de minishell projeniz için kapsamlı test araçları bulunmaktadır.

## 🚀 Test Araçları

### 1. `advanced_test.sh` - Gelişmiş Test Runner (ÖNERİLEN)

Interactive minishell'ler için özel olarak tasarlanmış, banner ve prompt'ları otomatik olarak temizler.

```bash
./advanced_test.sh                 # Tüm testleri çalıştır
./advanced_test.sh 1 2             # Sadece 1 ve 2 numaralı testleri çalıştır
./advanced_test.sh -v 0            # Parsing testlerini verbose modda çalıştır
./advanced_test.sh --quiet         # Sadece özet göster
```

### 2. `run_tests.sh` - Kapsamlı Test Runner

Tüm test dosyalarını çalıştırır ve bash ile karşılaştırma yapar.

```bash
./run_tests.sh                 # Tüm testleri çalıştır
./run_tests.sh -v              # Verbose mod (detaylı çıktı)
./run_tests.sh --no-diffs      # Diff çıktısı gösterme
```

### 3. `quick_test.sh` - Hızlı Test Runner

Belirli bir test dosyasını çalıştırır.

```bash
./quick_test.sh 1              # 1_scmds.sh testini çalıştır
./quick_test.sh 2 -v           # 1_builtins.sh testini verbose modda çalıştır
```

### 4. `make_test_friendly.sh` - Minishell'i Test Dostu Yapma Rehberi

Minishell'inizi test dostu hale getirmek için kod değişikliklerini gösterir.

```bash
./make_test_friendly.sh        # Interaktif rehber
```

## 📋 Mevcut Testler

- `0` - Parsing karşılaştırması (0_compare_parsing.sh)
- `1` - Basit komutlar (1_scmds.sh)
- `2` - Built-in komutlar (1_builtins.sh)
- `3` - Değişkenler (1_variables.sh)
- `4` - Yönlendirmeler (1_redirs.sh)
- `5` - Pipeline'lar (1_pipelines.sh)
- `6` - Düzeltme testleri (2_correction.sh)
- `7` - Path kontrolü (2_path_check.sh)
- `8` - Syntax hataları (8_syntax_errors.sh)
- `9` - Gelişmiş testler (9_go_wild.sh)
- `10` - Parsing zorluklari (10_parsing_hell.sh)

## Kullanım Örnekleri

### Tüm testleri çalıştır:
```bash
./run_tests.sh
```

### Sadece built-in testlerini çalıştır:
```bash
./quick_test.sh 2
```

### Verbose modda pipeline testleri:
```bash
./quick_test.sh 5 -v
```

### Sadece komutları görüntüle:
```bash
./quick_test.sh 1 -c
```

## Test Sonuçları

Test sonuçları şu şekilde gösterilir:
- ✅ **PASS**: Test başarılı
- ❌ **FAIL**: Test başarısız
- **INFO**: Bilgilendirme mesajı
- **WARN**: Uyarı mesajı

## Otomatik Derleme

Test scriptleri `./minishell` dosyasını bulamadığında otomatik olarak `make` komutu çalıştırır.

## Gereksinimler

- Bash shell
- `make` komutu
- `timeout` komutu
- `diff` komutu

## Sorun Giderme

### Minishell bulunamıyor hatası:
```bash
make clean && make
```

### Permission denied hatası:
```bash
chmod +x run_tests.sh quick_test.sh
```

### Test dosyası bulunamıyor hatası:
Test dosyalarının `tester/` dizininde olduğundan emin olun.

## Test Kategorileri

1. **Parsing Tests** - Input parsing ve tokenization
2. **Builtin Tests** - echo, cd, pwd, export, unset, env, exit
3. **Simple Commands** - Basit komut çalıştırma
4. **Variables** - Environment değişkenleri ve expansion
5. **Redirections** - Input/output yönlendirme
6. **Pipelines** - Pipe işlemleri
7. **Syntax Errors** - Hatalı syntax durumları
8. **Advanced Tests** - Karmaşık senaryolar

## İpuçları

- Belirli bir test kategorisinde sorun yaşıyorsanız önce o kategoriyi tek başına test edin
- Verbose mod (-v) kullanarak ne tür farklılıklar olduğunu görebilirsiniz
- Test başarısızlığı durumunda bash çıktısı ile minishell çıktısını karşılaştırın
