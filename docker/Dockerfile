FROM sptrakesh/boost as base

RUN apk add --no-cache readline-dev

COPY CMakeLists.txt /opt/spt/encrypter/
COPY EncrypterConfig.cmake /opt/spt/encrypter/
COPY src /opt/spt/encrypter/src
COPY docker/env.sh /opt/spt/encrypter/docker/

WORKDIR /opt/spt/encrypter
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/opt/local -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=mold" -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=mold" -B build -S . \
  && cmake --build build -j8 \
  && cmake --install build

FROM sptrakesh/cppruntime

COPY --from=base /opt/spt/bin /opt/spt/bin
COPY --from=base /opt/spt/include /opt/spt/include
COPY --from=base /opt/spt/lib /opt/spt/lib
COPY docker/scripts/*.sh /opt/spt/bin/

RUN apk add --no-cache readline \
  && addgroup spt \
  && adduser -DS -g "SPT User" -G spt -s /bin/sh -h /home/spt spt \
  && mkdir -p /opt/spt/logs /opt/spt/data \
  && chown spt:spt /opt/spt/logs /opt/spt/data

WORKDIR /opt/spt
ENV LD_LIBRARY_PATH=/usr/lib \
  PATH=$PATH:/opt/spt/bin
USER spt
ENTRYPOINT [ "/opt/spt/bin/entrypoint.sh" ]
