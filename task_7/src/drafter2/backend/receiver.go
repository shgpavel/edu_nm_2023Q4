// SPDX-License-Identifier: Apache-2.0

package main

import (
	"crypto/tls"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"

	"github.com/quic-go/quic-go"
	"github.com/quic-go/quic-go/http3"
)

func contains(str string, slice []string) bool {
	for _, s := range slice {
		if s == str {
			return true
		}
	}
	return false
}

func main() {
	availPaths := []string{"/init", "/add", "/clear", "/plot"}

	if len(os.Args) < 2 || !contains(os.Args[1], availPaths) {
		fmt.Printf("Usage: %s <path>\n", os.Args[0])
		fmt.Println("Paths: /init, /add, /plot, /clear")
		return
	}
	path := os.Args[1]

	tlsConfig := &tls.Config{
		InsecureSkipVerify: true,
		NextProtos:         []string{"quic"},
	}

	roundTripper := &http3.RoundTripper{
		TLSClientConfig: tlsConfig,
		QUICConfig:      &quic.Config{},
	}
	defer roundTripper.Close()
	client := &http.Client{
		Transport: roundTripper,
	}

	baseDial := "https://127.0.0.1:4433"
	resp, err := client.Get(baseDial + path)
	if err != nil {
		fmt.Println(err)
		return
	}
	defer resp.Body.Close()

	if resp.StatusCode == http.StatusOK {
		fmt.Println(resp.Status)

		body, err := ioutil.ReadAll(resp.Body)
		if err != nil {
			fmt.Println(err)
			return
		}
		fmt.Printf("Response Body: %s", string(body))
	} else {
		fmt.Println(resp.Status)
	}
}
