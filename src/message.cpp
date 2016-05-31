#include "message.h"

using std::string;

namespace cppkafka {

void dummy_deleter(rd_kafka_message_t*) {

}

Message Message::make_non_owning(rd_kafka_message_t* handle) {
    return Message(handle, NonOwningTag());
}

Message::Message() 
: handle_(nullptr, nullptr) {

}

Message::Message(rd_kafka_message_t* handle) 
: Message(HandlePtr(handle, &rd_kafka_message_destroy)) {

}

Message::Message(rd_kafka_message_t* handle, NonOwningTag) 
: Message(HandlePtr(handle, &dummy_deleter)) {

}

Message::Message(HandlePtr handle) 
: handle_(move(handle)), 
  payload_((const Buffer::DataType*)handle_->payload, handle_->len),
  key_((const Buffer::DataType*)handle_->key, handle_->key_len) {

}

bool Message::has_error() const {
    return get_error() != RD_KAFKA_RESP_ERR_NO_ERROR;
}

rd_kafka_resp_err_t Message::get_error() const {
    return handle_->err;
}

int Message::get_partition() const {
    return handle_->partition;
}

string Message::get_topic() const {
    return rd_kafka_topic_name(handle_->rkt);
}

const Buffer& Message::get_payload() const {
    return payload_;
}

const Buffer& Message::get_key() const {
    return key_;
}

int64_t Message::get_offset() const {
    return handle_->offset;
}

void* Message::private_data() {
    return handle_->_private;
}

Message::operator bool() const {
    return handle_ != nullptr;
}

rd_kafka_message_t* Message::get_handle() const {
    return handle_.get();
}

} // cppkafka
