ls *html | awk '{print "  <url>\n\t<loc>http://recomb2023.bilkent.edu.tr/"$1"</loc>\n\t<lastmod>2022-07-01</lastmod>\n\t<changefreq>daily</changefreq>\n\t<priority>1.0</priority>\n  </url>"}'
