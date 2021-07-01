import fs from "fs";

/**
 * 使用从ida复制出来的数据块，生成C格式的数组。
 * 参见 d2loader 工程的全局变量 extern union program_setting_store* global_dd_408620_settings;
 */
export function convertStringIndexTable(): void {
  let filename: string = "data/string-index-table.txt";
  let s = fs.readFileSync(filename, {
    encoding: "utf-8",
  });
  //console.log(s);
  let lines = s.split('\r\n');
  let buffer: number[] = [];
  //console.log(lines);
  for (let line of lines) {
    if (line.length > 0) {
      let splits = line.split('  ');
      let bytes = splits[1].split(' ');
      for (let b of bytes) {
        buffer.push(Number.parseInt(b, 16));
      }
      bytes = splits[2].split(' ');
      for (let b of bytes) {
        buffer.push(Number.parseInt(b, 16));
      }
    }
  }
  //console.log(buffer);
  buffer = buffer.slice(8);
  const baseAddress = 0x00402EA8;
  for (let i = 0; i < buffer.length; i+= 20) {
    if (getUint32At(i) == 0 &&
      getUint32At(i + 4) == 0 &&
      getUint32At(i + 8) == 0 &&
      getUint32At(i + 12) == 0 &&
      getUint32At(i + 16) == 0) {
        outputLastItem();
        break;
    } else {
      outputItemAt(i);
    }
  }

  function outputItemAt(index: number): void {
    let ptr = getUint32At(index);
    ptr -= baseAddress;
    let shortName = getAsciiStringAt(ptr);
    ptr = getUint32At(index + 4);
    ptr -= baseAddress;
    let category = getAsciiStringAt(ptr);
    ptr = getUint32At(index + 8);
    ptr -= baseAddress;
    let longName = getAsciiStringAt(ptr);
    let length = getUint32At(index + 12);
    let type = getUint32At(index + 16);
    let result = `{ ${JSON.stringify(shortName)}, ${JSON.stringify(category)}, ${JSON.stringify(longName)}, 0x${length.toString(16)}, ${formatArgumentType(type)} },`;
    console.log(result);
  }

  function formatArgumentType(type: number): string {
    switch (type) {
      case 1:
        return "ARG_TYPE_NO_PARAM";
      case 2:
        return "ARG_TYPE_UNSIGNED_SHORT_INT";
      case 4:
          return "ARG_TYPE_UNSIGNED_INT";
      default:
        return "0x" + type.toString(16);
    }
  }

  function getAsciiStringAt(index: number): string {
    let s = "";
    while (buffer[index] != 0) {
      s += String.fromCodePoint(buffer[index]);
      index++;
    }
    return s;
  }

  function outputLastItem(): void {
    let s = "{ NULL, NULL, NULL, 0, 0 },";
    console.log(s);
  }

  function getUint32At(index: number): number {
    let result = 0;
    for (let i = 3; i >= 0; i--) {
      result = result * 256 + buffer[index + i];
    }
    return result;
  }
}
