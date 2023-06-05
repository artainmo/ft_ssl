echo '> ./../ft_ssl'
./../ft_ssl
echo ""
echo '> ./../ft_ssl foobar'
./../ft_ssl foobar
echo ""
echo ""
echo ""
echo ""
echo '> echo "42 is nice" | ./../ft_ssl md5'
echo "42 is nice" | ./../ft_ssl md5
echo '> echo "42 is nice" | openssl md5'
echo "42 is nice" | openssl md5
echo ""
echo '> echo "Pity the living." | ./../ft_ssl md5'
echo "Pity the living." | ./../ft_ssl md5
echo '> echo "Pity the living." | openssl md5'
echo "Pity the living." | openssl md5
echo ""
echo '> ./../ft_ssl md5 file'
./../ft_ssl md5 file
echo '> openssl md5 file'
openssl md5 file
echo ""
echo '> echo "42 is nice" | ./../ft_ssl md5 -p'
echo "42 is nice" | ./../ft_ssl md5 -p
echo ""
echo '> echo "Pity the living." | ./../ft_ssl md5 -q -r'
echo "Pity the living." | ./../ft_ssl md5 -q -r
echo ""
echo '> ./../ft_ssl md5 -r file'
./../ft_ssl md5 -r file
echo ""
echo "> ./../ft_ssl md5 -s 'pity those that aren't following baerista on spotify.'"
./../ft_ssl md5 -s "pity those that aren't following baerista on spotify."
echo ""
echo '> echo "be sure to handle edge cases carefully" | ./../ft_ssl md5 -p file'
echo "be sure to handle edge cases carefully" | ./../ft_ssl md5 -p file
echo ""
echo '> echo "some of this will not make sense at first" | ./../ft_ssl md5 file'
echo "some of this will not make sense at first" | ./../ft_ssl md5 file
echo ""
echo '> echo "but eventually you will understand" | ./../ft_ssl md5 -p -r file'
echo "but eventually you will understand" | ./../ft_ssl md5 -p -r file
echo ""
echo "> echo 'GL HF let's go' | ./../ft_ssl md5 -p -s 'foo' file"
echo "GL HF let's go" | ./../ft_ssl md5 -p -s "foo" file
echo ""
echo '> echo "one more thing" | ./../ft_ssl md5 -r -p -s "foo" file -s "bar"'
echo "one more thing" | ./../ft_ssl md5 -r -p -s "foo" file -s "bar"
echo ""
echo '> echo "just to be extra clear" | ./../ft_ssl md5 -r -q -p -s "foo" file'
echo "just to be extra clear" | ./../ft_ssl md5 -r -q -p -s "foo" file
echo ""
echo '> echo "" | ./../ft_ssl md5 -p'
echo "" | ./../ft_ssl md5 -p
echo '> echo "" | openssl md5'
echo "" | openssl md5
echo ""
echo '> ./../ft_ssl md5 empty'
./../ft_ssl md5 empty
echo '> openssl md5 empty'
openssl md5 empty
echo ""
echo '> cat test.sh | ./../ft_ssl md5'
cat test.sh | ./../ft_ssl md5
echo '> cat test.sh | openssl md5'
cat test.sh | openssl md5
echo ""
echo '> ./../ft_ssl md5 test.sh'
./../ft_ssl md5 test.sh
echo '> openssl md5 test.sh'
openssl md5 test.sh
echo ""
echo ""
echo ""
echo ""
echo '> ./../ft_ssl sha256 -s "42 is nice"'
./../ft_ssl sha256 -s "42 is nice"
echo ""
echo '> ./../ft_ssl sha256 -q website'
./../ft_ssl sha256 -q website
echo ""
echo '> ./../ft_ssl sha256 test.sh'
./../ft_ssl sha256 test.sh
echo '> openssl sha256 test.sh'
openssl sha256 test.sh
echo ""
echo './../ft_ssl sha256 no'
./../ft_ssl sha256 no
echo ""
