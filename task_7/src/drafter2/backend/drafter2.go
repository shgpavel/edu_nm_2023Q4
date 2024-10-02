// SPDX-License-Identifier: Apache-2.0
package main

import (
	"crypto/tls"
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"

	"github.com/quic-go/quic-go"
	"github.com/quic-go/quic-go/http3"
)

func loadTLS(certFile, keyFile string) *tls.Config {
	cert, err := tls.LoadX509KeyPair(certFile, keyFile)
	if err != nil {
		log.Fatal("drafter server: Failed to load tls certificates: %v", err)
	}
	return &tls.Config{
		Certificates: []tls.Certificate{cert},
		NextProtos:   []string{"quic"},
	}
}

func serveJSHandler(filename string) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/javascript")
		http.ServeFile(w, r, filename)
	}
}

func main() {
	certFile := flag.String("c", "", "Path to the certificate file (cert.pem)")
	keyFile := flag.String("k", "", "Path to the key file (key.pem)")
	flag.Parse()

	if *certFile == "" || *keyFile == "" {
		fmt.Printf("Usage: %s -c cert -k key\n", os.Args[0])
		return
	}

	mux := http.NewServeMux()
	mux.HandleFunc("/init", serveJSHandler("js/init.js"))
	mux.HandleFunc("/add", serveJSHandler("js/add.js"))
	mux.HandleFunc("/plot", serveJSHandler("js/plot.js"))
	mux.HandleFunc("/clear", serveJSHandler("js/clear.js"))

	tlsConfig := loadTLS(*certFile, *keyFile)

	server := http3.Server{
		Handler:    mux,
		Addr:       ":4433",
		TLSConfig:  http3.ConfigureTLSConfig(tlsConfig),
		QUICConfig: &quic.Config{},
	}

	log.Println("[drafter2] QUIC server is running on port 4433")
	err := server.ListenAndServe()
	if err != nil {
		log.Fatal(err)
	}
}
