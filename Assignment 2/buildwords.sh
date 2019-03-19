#! /bin/bash

# select <td> tags
grep -o '<td>.*</td>' |

# remove all the tags   
sed 's/<[^>]*>//g' |

# squeeze new lines
tr -s '\n' |

# select even lines
sed -n '0~2p' |

# replace , with new line 
sed 's/,/\n/g' |

# replace a space with new line
sed 's/ /\n/g' |

# replace ` with ' 
sed "s/\`/\'/g" |

# translate uppercase into lowercase
tr '[:upper:]' '[:lower:]' |

# remove non-Hawaiian words
sed /[^pk\'mnwlhaeiou]/d |

# squeeze new lines
tr -s '\n' |

# sort with unique option
sort -u