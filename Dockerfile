FROM gcc:13.2 as builder

WORKDIR /build

COPY . .

RUN make clean && make

RUN chmod +x /build/bin/chatpug

FROM scratch

COPY --from=builder /build/bin/chatpug /

EXPOSE 9999

ENTRYPOINT ["/chatpug"]

CMD [ "-s" ]